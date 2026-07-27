#pragma once

#include <string>
#include "engine.h"

enum class GameState {
	MENU,
	PAUSE,
	QUIT,
	INGAME,
	IDLE,
	DEV
};

class Game
{
public:
	void Init();
	GameState getGameState() const { return g_state; }
	void setGameState(GameState state) { g_state = state; }
private:
	GameState g_state = GameState::IDLE;
};

inline std::string gameStateToString(GameState state) {
	switch (state) {
	case GameState::MENU:   return "MENU";
	case GameState::PAUSE:  return "PAUSE";
	case GameState::QUIT:   return "QUIT";
	case GameState::INGAME: return "INGAME";
	case GameState::IDLE:   return "IDLE";
	case GameState::DEV:    return "DEV";
	default:                return "UNKNOWN";
	}
}