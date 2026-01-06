#include "core/model.h"

Mesh* Model::LoadFromFile(const std::string& filePath, const char* vertShader, const char* fragShader, const char* texturePath, GLenum format, Camera* camera, glm::vec3 position, glm::vec3 size)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

	if (!scene || !scene->HasMeshes())
	{
		//Log(error, ("Assimp Failed To Load Model: " + filePath).c_str());
		return nullptr;
	}

	for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		aiMesh* mesh = scene->mMeshes[m];
		unsigned int vertexOffset = 0;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			
			if (mesh->HasNormals())
				vertex.color = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			else
				vertex.color = { 1.0f, 1.0f, 1.0f };

			if (mesh->mTextureCoords[0])
				vertex.texture = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			else
				vertex.texture = { 0.0f, 0.0f };

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(vertexOffset + face.mIndices[j]);
		}

		vertexOffset += mesh->mNumVertices;

		meshes.push_back(new Mesh(vertices, indices, vertShader, fragShader, texturePath, format, camera, position, size));
	}

	return meshes.empty() ? nullptr : meshes[0];
}

void Model::Draw()
{
	for (auto mesh : meshes)
		mesh->Draw();
}

void Model::Shutdown()
{
	for (auto mesh : meshes)
		delete mesh;

	meshes.clear();
}