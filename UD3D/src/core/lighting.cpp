#include "core/lighting.h"

Lighting::Lighting(const std::vector<Vertex>& lightVert, glm::vec3 lightPos, glm::vec3 lightSize, glm::vec4 lightColor, Camera* cam, const char* vertShader, const char* fragShader)
	: lightPos(lightPos), lightSize(lightSize), lightColor(lightColor), camera(cam), vertexCount(lightVert.size())
{
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, lightVert.size() * sizeof(Vertex), lightVert.data(), GL_STATIC_DRAW);

	shader.LoadShaderProgramFromFile(vertShader, fragShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

Lighting::~Lighting()
{
	glDeleteBuffers(1, &lightVBO);
	glDeleteVertexArrays(1, &lightVAO);
}

void Lighting::Draw()
{
	shader.Bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), lightSize);

	shader.SetMat4("model", model);
	shader.SetMat4("projection", camera->GetProjectionMatrix());
	shader.SetMat4("view", camera->GetViewMatrix());
	shader.SetVec4("lightColor", lightColor);
	shader.SetVec3("lightPos", lightPos);

	glBindVertexArray(getVAO());
	glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
	glBindVertexArray(0);
}