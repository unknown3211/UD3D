#pragma once

#include "core/mesh.h"
#include "core/model.h"
#include "core/lighting.h"
#include "cameras/camera.h"

class SceneManager
{
public:
	void Update(float deltaTime);
	void Draw(const Lighting& light);
	void Shutdown();
	void AddMesh(Mesh* mesh);
	void AddModel(Model* model);
	void debugMode(bool debugmode);
	bool getDebugMode() const { return debug; }
private:
	std::vector<Mesh*> meshes;
	std::vector<Model*> models;
	bool debug;
};
