#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include "core/shaders.h"
#include "cameras/camera.h"
#include "globals/global_structs.h"
#include "buffers/vbo.h"
#include "buffers/vao.h"
#include "buffers/ebo.h"

class Skybox
{
public:
	void Load(const std::string faces[6]);
	void Draw(int width, int height, Shader& shader, Camera& camera);
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	std::unique_ptr<VAO> vao;
	std::unique_ptr<VBO> vbo;
	std::unique_ptr<EBO> ebo;

	GLuint cubemapTexture = 0;
};