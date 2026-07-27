#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "mesh.h"
#include "shaders.h"
#include "textures.h"
#include "cameras/camera.h"
#include "lighting.h"
#include "utils/glm_includes.h"
#include "globals/global_structs.h"
#include "animation/animation.h"
#include "animation/animator.h"

class Model
{
public:
	Model() = default;
	~Model() { Shutdown(); }

	Mesh* LoadFromFile(const std::string& filePath, const char* vertShader, const char* fragShader, const char* defaultTexturePath, GLenum format, Camera* camera, glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));
	
	void UpdateAnimation(float dt);
	void PlayAnimation(int animationIndex);
	bool HasAnimations() const { return !m_animations.empty(); }
	Animator* GetAnimator() const { return m_animator.get(); }

	void Draw(const Lighting& light);
	void SetPosition(const glm::vec3& position);
	void SetSize(const glm::vec3& size);
	void SetRotation(const glm::quat& rotation);
	void SetTransformMatrix(const glm::mat4& transform);
	void SetViewMatrixOverride(const glm::mat4& view);
	void SetProjectionOverride(const glm::mat4& proj);
	void ClearViewOverrides();

	void Shutdown();

private:
	void ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory, const char* vertShader, const char* fragShader, const char* defaultTexturePath, GLenum format, Camera* camera, glm::vec3 position, glm::vec3 size, const glm::mat4& parentTransform);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, const char* vertShader, const char* fragShader, const char* defaultTexturePath, GLenum format, Camera* camera, glm::vec3 position, glm::vec3 size, const glm::mat4& nodeMatrix);
	
	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
	int m_boneCounter = 0;

	std::vector<AnimationClip> m_animations;
	std::unique_ptr<Animator> m_animator;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_loadedTextures;
};