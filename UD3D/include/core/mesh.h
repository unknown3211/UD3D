#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "shaders.h"
#include "textures.h"
#include "buffers/vbo.h"
#include "buffers/vao.h"
#include "buffers/ebo.h"
#include "globals/global_structs.h"
#include "cameras/camera.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& vertexShader, const std::string& fragmentShader, const std::string& texturePath, GLenum format,
		Camera* cam, glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));
	
	~Mesh();

	void Draw();
	void SetPosition(const glm::vec3& position) { m_pos = position; }
	void SetSize(const glm::vec3& size) { m_size = size; }
	const Texture* GetTexture() const { return texture.get(); }
	const Shader& GetShader() const { return shader; }
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	std::unique_ptr<VAO> vao;
	std::unique_ptr<VBO> vbo;
	std::unique_ptr<EBO> ebo;
	
	Shader shader;
	Camera* camera;
	std::unique_ptr<Texture> texture;
	
	glm::vec3 m_pos;
	glm::vec3 m_size;
};