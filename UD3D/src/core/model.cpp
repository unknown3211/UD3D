#include "core/model.h"
#include "logs.h"
#include <filesystem>
#include <iostream>

static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
	vertex.boneIds = glm::vec4(-1.0f);
	vertex.boneWeights = glm::vec4(0.0f);
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < 4; ++i)
	{
		if (vertex.boneIds[i] < 0.0f)
		{
			vertex.boneIds[i] = static_cast<float>(boneID);
			vertex.boneWeights[i] = weight;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (m_boneInfoMap.find(boneName) == m_boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = m_boneCounter;
			newBoneInfo.offset = ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			m_boneInfoMap[boneName] = newBoneInfo;
			boneID = m_boneCounter;
			m_boneCounter++;
		}
		else
		{
			boneID = m_boneInfoMap[boneName].id;
		}

		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			if (vertexId < static_cast<int>(vertices.size()))
			{
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}
	}
}

Mesh* Model::LoadFromFile(const std::string& filePath, const char* vertShader, const char* fragShader, const char* defaultTexturePath, GLenum format, Camera* camera, glm::vec3 position, glm::vec3 size)
{
	Shutdown();
	m_boneInfoMap.clear();
	m_boneCounter = 0;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices |
		aiProcess_LimitBoneWeights);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log(error, ("Assimp Failed To Load Model: " + filePath + " | Error: " + std::string(importer.GetErrorString())).c_str());
		return nullptr;
	}

	std::string directory = "";
	size_t lastSlash = filePath.find_last_of("/\\");
	if (lastSlash != std::string::npos)
	{
		directory = filePath.substr(0, lastSlash + 1);
	}

	ProcessNode(scene->mRootNode, scene, directory, vertShader, fragShader, defaultTexturePath, format, camera, position, size, glm::mat4(1.0f));

	if (scene->HasAnimations())
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			m_animations.emplace_back(filePath, scene, i, m_boneInfoMap, m_boneCounter);
		}
		if (!m_animations.empty())
		{
			m_animator = std::make_unique<Animator>(&m_animations[0]);
			auto transforms = m_animator->GetFinalBoneMatrices();
			for (auto mesh : meshes)
			{
				mesh->SetBoneMatrices(transforms);
			}
			Log(info, ("Loaded " + std::to_string(m_animations.size()) + " animation clip(s) for model: " + filePath).c_str());
		}
	}

	return meshes.empty() ? nullptr : meshes[0];
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory, const char* vertShader, const char* fragShader, const char* defaultTexturePath, GLenum format, Camera* camera, glm::vec3 position, glm::vec3 size, const glm::mat4& parentTransform)
{
	glm::mat4 nodeMatrix = parentTransform * ConvertMatrixToGLMFormat(node->mTransformation);

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, directory, vertShader, fragShader, defaultTexturePath, format, camera, position, size, nodeMatrix));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, directory, vertShader, fragShader, defaultTexturePath, format, camera, position, size, nodeMatrix);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, const char* vertShader, const char* fragShader, const char* defaultTexturePath, GLenum format, Camera* camera, glm::vec3 position, glm::vec3 size, const glm::mat4& nodeMatrix)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	bool hasMeshBones = (mesh->mNumBones > 0);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);

		glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		glm::vec3 norm(0.0f, 1.0f, 0.0f);
		if (mesh->HasNormals())
			norm = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (!hasMeshBones)
		{
			glm::vec4 tPos = nodeMatrix * glm::vec4(pos, 1.0f);
			pos = glm::vec3(tPos);
			glm::vec3 tNorm = glm::mat3(glm::transpose(glm::inverse(nodeMatrix))) * norm;
			norm = glm::normalize(tNorm);
		}

		vertex.position = pos;
		vertex.normal = norm;

		if (mesh->mTextureCoords[0])
			vertex.texture = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.texture = glm::vec2(0.0f, 0.0f);

		if (mesh->HasVertexColors(0))
			vertex.color = glm::vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b);
		else
			vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (hasMeshBones)
	{
		ExtractBoneWeightForVertices(vertices, mesh, scene);
	}

	std::unique_ptr<Texture> embeddedTexture = nullptr;
	std::string texPath = defaultTexturePath ? defaultTexturePath : "";

	if (mesh->mMaterialIndex >= 0 && scene->HasMaterials())
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString str;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS ||
		    material->GetTexture(aiTextureType_BASE_COLOR, 0, &str) == AI_SUCCESS ||
		    material->GetTexture(aiTextureType_UNKNOWN, 0, &str) == AI_SUCCESS)
		{
			std::string texName = str.C_Str();
			const aiTexture* aiTex = scene->GetEmbeddedTexture(texName.c_str());
			if (!aiTex && !texName.empty() && texName[0] == '*')
			{
				int texIdx = std::atoi(texName.c_str() + 1);
				if (texIdx >= 0 && texIdx < static_cast<int>(scene->mNumTextures))
				{
					aiTex = scene->mTextures[texIdx];
				}
			}

			if (aiTex)
			{
				if (aiTex->mHeight == 0)
				{
					embeddedTexture = std::make_unique<Texture>(reinterpret_cast<const unsigned char*>(aiTex->pcData), aiTex->mWidth, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
				}
			}
			else
			{
				std::string fullPath = directory + texName;
				if (std::filesystem::exists(fullPath))
				{
					texPath = fullPath;
				}
			}
		}
	}

	Mesh* createdMesh = new Mesh(vertices, indices, vertShader, fragShader, texPath, format, camera, position, size);
	createdMesh->SetHasSkinning(hasMeshBones);
	if (embeddedTexture)
	{
		createdMesh->SetTexture(std::move(embeddedTexture));
	}
	return createdMesh;
}

void Model::UpdateAnimation(float dt)
{
	if (m_animator)
	{
		m_animator->UpdateAnimation(dt);
		auto transforms = m_animator->GetFinalBoneMatrices();
		for (auto mesh : meshes)
		{
			mesh->SetBoneMatrices(transforms);
		}
	}
}

void Model::PlayAnimation(int animationIndex)
{
	if (m_animator && animationIndex >= 0 && animationIndex < static_cast<int>(m_animations.size()))
	{
		m_animator->PlayAnimation(&m_animations[animationIndex]);
	}
}

void Model::Draw(const Lighting& light)
{
	for (auto mesh : meshes)
	{
		mesh->Draw(light);
	}
}

void Model::SetPosition(const glm::vec3& position)
{
	for (auto mesh : meshes)
		mesh->SetPosition(position);
}

void Model::SetSize(const glm::vec3& size)
{
	for (auto mesh : meshes)
		mesh->SetSize(size);
}

void Model::SetRotation(const glm::quat& rotation)
{
	for (auto mesh : meshes)
		mesh->SetRotation(rotation);
}

void Model::SetTransformMatrix(const glm::mat4& transform)
{
	for (auto mesh : meshes)
		mesh->SetTransformMatrix(transform);
}

void Model::SetViewMatrixOverride(const glm::mat4& view)
{
	for (auto mesh : meshes)
		mesh->SetViewMatrixOverride(view);
}

void Model::SetProjectionOverride(const glm::mat4& proj)
{
	for (auto mesh : meshes)
		mesh->SetProjectionOverride(proj);
}

void Model::ClearViewOverrides()
{
	for (auto mesh : meshes)
		mesh->ClearViewOverrides();
}

void Model::Shutdown()
{
	for (auto mesh : meshes)
		delete mesh;

	meshes.clear();
	m_animations.clear();
	m_animator.reset();
}