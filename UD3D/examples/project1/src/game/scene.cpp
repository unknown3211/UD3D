#include "scene.h"
#include <string>
#include "misc/skybox.h"

Scene::Scene(Camera* camera, bool debugmode) : debug(debugmode), cam(*camera)
{
	if (debug)
	{
		scene_mngr.debugMode(true);
	}

	OnInit(camera);
}

Scene::~Scene()
{
	scene_mngr.Shutdown();
}

void Scene::Draw(Window& window)
{
	scene_mngr.Draw();
	//skybox.Draw(window.GetDetails().s_width, window.GetDetails().s_height, skyboxShader, cam);
}

void Scene::OnInit(Camera* camera)
{
	glm::vec3 groundPos = glm::vec3(-1.0f, 0.0f, 0.0f);
	LoadGround(groundPos, camera);
}

/// SCENE ///

void Scene::LoadGround(glm::vec3 position, Camera* camera)
{
	float s = 5.0f; // size
	float c = 1.0f; // color

	std::vector<Vertex> vertices =
	{
		{{-s, 0.0f, -s}, {c, c, c}, {0.0f, 0.0f}}, // bottom left
		{{ s, 0.0f, -s}, {c, c, c}, {1.0f, 0.0f}}, // bottom right
		{{ s, 0.0f,  s}, {c, c, c}, {1.0f, 1.0f}}, // top right
		{{-s, 0.0f,  s}, {c, c, c}, {0.0f, 1.0f}}, // top left
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	std::string vertPath = std::string(RESOURCES_PATH) + "shaders/default.vert";
	std::string fragPath = std::string(RESOURCES_PATH) + "shaders/default.frag";
	std::string texPath  = std::string(RESOURCES_PATH) + "textures/grass.jpg";

	scene_mngr.AddMesh(new Mesh(vertices, indices, vertPath, fragPath, texPath, GL_RGB, camera, position));
}

void Scene::LoadSkyBox()
{
	std::string faces[6] =
	{
		std::string(RESOURCES_PATH) + "skyboxes/sky/right.jpg",
		std::string(RESOURCES_PATH) + "skyboxes/sky/left.jpg",
		std::string(RESOURCES_PATH) + "skyboxes/sky/top.jpg",
		std::string(RESOURCES_PATH) + "skyboxes/sky/bottom.jpg",
		std::string(RESOURCES_PATH) + "skyboxes/sky/front.jpg",
		std::string(RESOURCES_PATH) + "skyboxes/sky/back.jpg"
	};

	skybox.Load(faces);
	skyboxShader.LoadShaderProgramFromFile(RESOURCES_PATH "shaders/skybox/skybox.vert", RESOURCES_PATH "shaders/skybox/skybox.frag");
}