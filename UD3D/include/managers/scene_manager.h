#pragma once

#include "core/mesh.h"
#include "core/model.h"
#include "cameras/camera.h"

class SceneManager
{
public:
	void Draw();
	void Shutdown();
	void AddMesh(Mesh* mesh);
	void AddModel(Model* model);
	void debugMode(bool debugmode);
	bool getDebugMode() { return debug; }
private:
	bool debug;
	std::vector<Mesh*> meshes;
	std::vector<Model*> models;
};