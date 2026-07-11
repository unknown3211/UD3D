#pragma once

#include <array>

class InputManager
{
public:
	InputManager() = default;
	void setKeyPressed(int key, bool pressed);
	bool isKeyPressed(int key);
private:
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator = (const InputManager&) = delete;
	InputManager& operator = (InputManager&) = delete;
private:
	std::array<bool, 349> m_keys = { false };
};