#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
private:
	void Init();
	void Update();

	glm::vec3 m_pos;
	glm::vec3 m_target;
	glm::vec3 m_up;

	float m_speed = 1.0f;

	float m_AngleH;
	float m_AngleV;

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnLeftEdge;
	bool m_OnRightEdge;

	glm::ivec2 m_mousePos;
};