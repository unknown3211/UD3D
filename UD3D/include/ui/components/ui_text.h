#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <string>
#include <map>
#include <memory>

#include "buffers/vao.h"
#include "buffers/vbo.h"
#include "core/shaders.h"
#include "cameras/camera.h"
#include "utils/glm_includes.h"

struct Character {
	unsigned int textureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class UIText {
public:
	void initText(std::string fontPath, int fontSize);
	void renderText(Shader& shader, std::string text, glm::vec2 position, float scale, glm::vec3 color, int s_width, int s_height);
private:
	FT_Library ft;
	FT_Face face;
	
	std::map<char, Character> characters;

	std::unique_ptr<VAO> vao;
	std::unique_ptr<VBO> vbo;

	Shader shader;
};