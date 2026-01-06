#include <iostream>
#include "game.h"
#include "engine.h"
#include "scene.h"

Window window;
WindowDetails details;
Game game;
Scene* level;
FCamera free_cam;

static void OnStart()
{
	details.s_width = 1920;
	details.s_height = 1080;
	details.title = "3D Framework";
	details.vSync = true;
	details.dev = true;
	window.CreateWindow(details);
	window.SetBackgroundColor({ 0.0f, 0.25f, 0.0f, 0.5f });

	game.setGameState(GameState::DEV);
	std::string gs = gameStateToString(game.getGameState());
	//Log(info, ("GameState: " + gs).c_str());

	free_cam.Activate(glm::vec3(-1.0f, 5.0f, 12.0f));
	level = new Scene(&free_cam.GetCam(), false);
}

static void OnUpdate()
{
	float deltaTime = DeltaTime();
	free_cam.Update(window.GetGLFWwindow(), deltaTime);
	level->Draw(window);
}

static void OnShutdown()
{
	free_cam.Deactivate();
	
	delete level;
	level = nullptr;
}

void Game::Init()
{
	window.Start(OnStart);
	window.Update(OnUpdate);
	window.Shutdown(OnShutdown);
}