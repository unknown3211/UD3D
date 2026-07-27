#pragma once

#include <glad/glad.h>
#include <vector>
#include <memory>
#include "shaders.h"
#include "textures.h"
#include "buffers/vbo.h"
#include "buffers/vao.h"
#include "buffers/ebo.h"
#include "globals/global_structs.h"
#include "cameras/camera.h"
#include "lighting.h"
#include "utils/glm_includes.h"
#include <glm/gtc/quaternion.hpp>

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& vertexShader, const std::string& fragmentShader, const std::string& texturePath, GLenum format,
		Camera* cam, glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));
	
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, Shader shader, Texture* texture,
		Camera* cam, glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));

	~Mesh();

	void Draw(const Lighting& light);
	void SetPosition(const glm::vec3& position) { m_pos = position; }
	void SetSize(const glm::vec3& size) { m_size = size; }
	void SetRotation(const glm::quat& rotation) { m_rotation = rotation; m_hasRotation = true; }
	void SetTransformMatrix(const glm::mat4& transform) { m_transformMatrix = transform; m_useCustomMatrix = true; }
	void SetBoneMatrices(const std::vector<glm::mat4>& boneMatrices) { m_boneMatrices = boneMatrices; }
	void SetHasSkinning(bool hasSkinning) { m_hasSkinning = hasSkinning; }
	bool HasSkinning() const { return m_hasSkinning; }
	void SetTexture(std::unique_ptr<Texture> tex) { texture = std::move(tex); }
	void SetViewMatrixOverride(const glm::mat4& view) { m_viewOverride = view; m_hasViewOverride = true; }
	void SetProjectionOverride(const glm::mat4& proj) { m_projOverride = proj; m_hasProjOverride = true; }
	void ClearViewOverrides() { m_hasViewOverride = false; m_hasProjOverride = false; }

	const Texture* GetTexture() const { return texture.get(); }
	const Shader& GetShader() const { return shader; }

private:
	void SetupMesh();

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	std::unique_ptr<VAO> vao;
	std::unique_ptr<VBO> vbo;
	std::unique_ptr<EBO> ebo;
	
	Shader shader;
	Camera* camera = nullptr;
	std::unique_ptr<Texture> texture;
	Texture* m_externalTexture = nullptr;
	
	glm::vec3 m_pos{ 0.0f };
	glm::vec3 m_size{ 1.0f };
	glm::quat m_rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::mat4 m_transformMatrix{ 1.0f };
	glm::mat4 m_viewOverride{ 1.0f };
	glm::mat4 m_projOverride{ 1.0f };
	bool m_hasRotation = false;
	bool m_useCustomMatrix = false;
	bool m_hasSkinning = false;
	bool m_hasViewOverride = false;
	bool m_hasProjOverride = false;

	std::vector<glm::mat4> m_boneMatrices;
};
