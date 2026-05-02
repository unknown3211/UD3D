#include "managers/ui_manager.h"

void UIManager::initText(std::string fontPath, int fontSize, std::string vertPath, std::string fragPath)
{
	ui_text.initText(fontPath, fontSize);
	text_shader.LoadShaderProgramFromFile(vertPath.c_str(), fragPath.c_str());;
}

void UIManager::renderText(Shader& shader, std::string text, glm::vec2 position, float size, glm::vec3 color, float s_width, float s_height)
{
	ui_text.renderText(shader, text, position, size, color, s_width, s_height);
}