#include "managers/input/input_manager.h"

void InputManager::setKeyPressed(int key, bool pressed)
{
	if (key < 0 || key >= (int)(m_keys.size()))
	{
		return;
	}

	m_keys[key] = pressed;
}

bool InputManager::isKeyPressed(int key)
{
	if (key < 0 || key >= (int)(m_keys.size()))
	{
		return false;
	}

	return m_keys[key];
}