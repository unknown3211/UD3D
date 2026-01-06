#pragma once

#include "engine.h"

class Scene {
public:
	Scene(Camera* camera, bool debugmode);
	~Scene();
	void OnInit(Camera* camera);
	void Draw(Window& window);
	void LoadGround(glm::vec3 position, Camera* camera);
	void LoadSkyBox();
private:
	SceneManager scene_mngr;
	Skybox skybox;
	Shader skyboxShader;
	Camera& cam;
	bool debug;
};