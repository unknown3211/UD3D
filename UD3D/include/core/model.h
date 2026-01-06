#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include "mesh.h"
#include "shaders.h"
#include "textures.h"
#include "cameras/camera.h"

class Model
{
public:
	Mesh* LoadFromFile(const std::string& filePath, const char* vertShader, const char* fragShader, const char* texturePath, GLenum format, Camera* camera, glm::vec3 position, glm::vec3 size);
	void Draw();
	void Shutdown();
private:
	std::vector<Mesh*> meshes;
};