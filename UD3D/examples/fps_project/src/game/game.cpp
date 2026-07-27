#include <iostream>
#include "game.h"
#include "engine.h"
#include "scene.h"
#include "logs.h"

Window window;
WindowDetails details;
Game game;
Scene* level;
Camera* activeCam;

static void OnStart()
{
	std::string settingsPath = std::string(CONFIGS_PATH) + "ini/game.ini";
	LoadGameConfig(details, settingsPath);

	window.CreateWindow(details);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game.setGameState(GameState::DEV);
	std::string gs = gameStateToString(game.getGameState());
	Log(info, ("GameState: " + gs).c_str());

	activeCam = &level->GetFPSController().GetCam();
	level = new Scene(activeCam, false);
}

static void OnUpdate()
{
	float deltaTime = DeltaTime();
	activeCam = &level->GetFPSController().GetCam();
	level->Draw(window, activeCam, deltaTime);
}

static void OnShutdown()
{
	delete level;
	delete activeCam;

	level = nullptr;
	activeCam = nullptr;
}

void Game::Init()
{
	window.Start(OnStart);
	window.Update(OnUpdate);
	window.Shutdown(OnShutdown);
}
