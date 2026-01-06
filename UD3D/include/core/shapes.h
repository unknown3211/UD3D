#pragma once

#include "mesh.h"
#include "shaders.h"
#include "buffers/vao.h"
#include "buffers/vbo.h"
#include "buffers/ebo.h"
#include "cameras/camera.h"

class Shape
{
public:
	void InitTriangle(float size, glm::vec3 color);
	void InitRectangle(float size, glm::vec3 color);
	void Draw();
	void Delete();
private:
	std::unique_ptr<VAO> vao;
	std::unique_ptr<VBO> vbo;
	std::unique_ptr<EBO> ebo;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	Shader shader;
	Camera g_camera;
};