#pragma once

#include "globals/global_types.h"
#include "utils/glm_includes.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
	glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::ivec4 boneIds = glm::ivec4(-1);
	glm::vec4 boneWeights = glm::vec4(0.0f);
};

struct Color {
	f32 r, g, b, a;
};

struct Vector2 {
	f32 x, y;
};

struct Vector3 {
	f32 x, y, z;
};
