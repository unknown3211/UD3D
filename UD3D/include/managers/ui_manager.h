#pragma once

#include "core/shaders.h"
#include "cameras/camera.h"
#include "ui/components/ui_text.h"
#include <string>

class UIManager {
public:
	void initText(std::string fontPath, int fontSize, std::string vertPath, std::string fragPath);
	void renderText(Shader& shader, std::string text, glm::vec2 position, float size, glm::vec3 color, float s_width, float s_height);
private:
	UIText ui_text;
	Shader text_shader;
};