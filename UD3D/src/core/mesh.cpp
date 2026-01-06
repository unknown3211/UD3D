#include "core/mesh.h"
#include <iostream>
#include <string>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& vertexShader, const std::string& fragmentShader, const std::string& texturePath, GLenum format,
	 Camera* cam, glm::vec3 position, glm::vec3 size)
   : vertices(vertices), indices(indices), camera(cam), m_pos(position), m_size(size)
{
	vao = std::make_unique<VAO>();
	vbo = std::make_unique<VBO>(vertices);
	ebo = std::make_unique<EBO>(indices);

	vao->Bind();

	shader.LoadShaderProgramFromFile(vertexShader.c_str(), fragmentShader.c_str());

	if (!texturePath.empty())
	{
		texture = std::make_unique<Texture>(texturePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, format, GL_UNSIGNED_BYTE);
	}

	GLsizei stride = sizeof(Vertex);
	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, stride, (void*)0);
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, stride, (void*)(3 * sizeof(float)));
	vao->LinkAttrib(*vbo, 2, 2, GL_FLOAT, stride, (void*)(6 * sizeof(float)));
	vao->LinkAttrib(*vbo, 3, 3, GL_FLOAT, stride, (void*)(8 * sizeof(float)));

	vao->Unbind();
}

Mesh::~Mesh() = default;

void Mesh::Draw()
{
	shader.Bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_pos) * glm::scale(glm::mat4(1.0f), m_size);
	shader.SetMat4("model", model);
	shader.SetMat4("projection", camera->GetProjectionMatrix());
	shader.SetMat4("view", camera->GetViewMatrix());

	if (texture) {
		texture->texUnit(shader, "tex0", 0);
		texture->Bind();
	}

	vao->Bind();
	ebo->Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	vao->Unbind();
}