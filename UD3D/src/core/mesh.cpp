#include "core/mesh.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& vertexShader, const std::string& fragmentShader, const std::string& texturePath, GLenum format,
	 Camera* cam, glm::vec3 position, glm::vec3 size)
   : vertices(vertices), indices(indices), camera(cam), m_pos(position), m_size(size)
{
	shader.LoadShaderProgramFromFile(vertexShader.c_str(), fragmentShader.c_str());

	if (!texturePath.empty())
	{
		texture = std::make_unique<Texture>(texturePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, format, GL_UNSIGNED_BYTE);
	}

	SetupMesh();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, Shader shader, Texture* texture,
	Camera* cam, glm::vec3 position, glm::vec3 size)
	: vertices(vertices), indices(indices), shader(shader), m_externalTexture(texture), camera(cam), m_pos(position), m_size(size)
{
	SetupMesh();
}

void Mesh::SetupMesh()
{
	vao = std::make_unique<VAO>();
	vbo = std::make_unique<VBO>(vertices);
	ebo = std::make_unique<EBO>(indices);

	vao->Bind();

	GLsizei stride = sizeof(Vertex);
	
	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, stride, (void*)offsetof(Vertex, position));
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, stride, (void*)offsetof(Vertex, color));
	vao->LinkAttrib(*vbo, 2, 2, GL_FLOAT, stride, (void*)offsetof(Vertex, texture));
	vao->LinkAttrib(*vbo, 3, 3, GL_FLOAT, stride, (void*)offsetof(Vertex, normal));
	vao->LinkAttrib(*vbo, 4, 4, GL_FLOAT, stride, (void*)offsetof(Vertex, boneIds));
	vao->LinkAttrib(*vbo, 5, 4, GL_FLOAT, stride, (void*)offsetof(Vertex, boneWeights));

	vao->Unbind();
}

Mesh::~Mesh() = default;

static GLuint GetFallbackWhiteTexture()
{
	static GLuint whiteTex = 0;
	if (whiteTex == 0)
	{
		glGenTextures(1, &whiteTex);
		glBindTexture(GL_TEXTURE_2D, whiteTex);
		unsigned char whitePixel[4] = { 255, 255, 255, 255 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return whiteTex;
}

void Mesh::Draw(const Lighting& light)
{
	shader.Bind();

	glm::mat4 model;
	if (m_useCustomMatrix)
	{
		model = m_transformMatrix;
	}
	else if (m_hasRotation)
	{
		glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), m_pos);
		glm::mat4 rotationMat = glm::mat4_cast(m_rotation);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), m_size);
		model = translationMat * rotationMat * scaleMat;
	}
	else
	{
		model = glm::translate(glm::mat4(1.0f), m_pos) * glm::scale(glm::mat4(1.0f), m_size);
	}
	
	shader.SetMat4("model", model);
	if (camera)
	{
		glm::mat4 proj = m_hasProjOverride ? m_projOverride : camera->GetProjectionMatrix();
		glm::mat4 view = m_hasViewOverride ? m_viewOverride : camera->GetViewMatrix();
		shader.SetMat4("projection", proj);
		shader.SetMat4("view", view);
	}

	shader.SetVec3("lightPos", light.getPosition());
	shader.SetVec4("lightColor", light.getColor());

	if (m_hasSkinning && !m_boneMatrices.empty())
	{
		for (size_t i = 0; i < m_boneMatrices.size() && i < 100; ++i)
		{
			std::string name = "bone_transforms[" + std::to_string(i) + "]";
			shader.SetMat4(name.c_str(), m_boneMatrices[i]);
		}
		shader.SetInt("hasBones", 1);
	}
	else
	{
		shader.SetInt("hasBones", 0);
	}

	if (texture) {
		texture->Bind();
		texture->texUnit(shader, "tex0", 0);
	}
	else if (m_externalTexture) {
		m_externalTexture->Bind();
		m_externalTexture->texUnit(shader, "tex0", 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GetFallbackWhiteTexture());
		shader.SetInt("tex0", 0);
	}

	vao->Bind();
	ebo->Bind();

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	
	vao->Unbind();
}
