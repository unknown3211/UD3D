#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "shaders.h"
#include "cameras/camera.h"
#include "lighting.h"
#include "globals/global_structs.h"
#include "utils/glm_includes.h"

class Lighting
{
public:
	Lighting(const std::vector<Vertex>& lightVert, glm::vec3 lightPos, glm::vec3 lightSize, glm::vec4 lightColor, Camera* cam, const char* vertShader, const char* fragShader);
	~Lighting();
	void Draw();

	GLuint getVAO() const { return lightVAO; }
	size_t getVertexCount() const { return vertexCount; }
	glm::vec3 getPosition() const { return lightPos; }
	glm::vec3 getSize() const { return lightSize; }
	glm::vec4 getColor() const { return lightColor; }
private:
	GLuint lightVAO, lightVBO;
	size_t vertexCount;
	Shader shader;
	Camera* camera;
	glm::vec3 lightPos;
	glm::vec3 lightSize;
	glm::vec4 lightColor;
};