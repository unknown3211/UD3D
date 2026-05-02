#include "managers/scene_manager.h"

void SceneManager::Update(float deltaTime)
{
	for (auto model : models)
	{
	}
}

void SceneManager::Draw(const Lighting& light)
{
	for (auto mesh : meshes)
		mesh->Draw(light);

	for (auto model : models)
		model->Draw(light);
}

void SceneManager::Shutdown()
{
	for (auto mesh : meshes)
		delete mesh;

	meshes.clear();

	for (auto model : models)
		delete model;

	models.clear();
}

void SceneManager::debugMode(bool debugmode)
{
	debug = debugmode;
	glPolygonMode(GL_FRONT_AND_BACK, debug ? GL_LINE : GL_FILL);
}

void SceneManager::AddMesh(Mesh* mesh)
{
	meshes.push_back(mesh);
}

void SceneManager::AddModel(Model* model)
{
	models.push_back(model);
}
