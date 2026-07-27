#pragma once

#include "globals/global_types.h"
#include "utils/glm_includes.h"
#include <string>
#include <vector>
#include <unordered_map>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
	glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec4 boneIds = glm::vec4(0);
	glm::vec4 boneWeights = glm::vec4(0.0f);
};

struct Bone
{
	int id = 0;
	std::string name = "";
	glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children = {};
};

struct BoneTransformTrack
{
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimetamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<glm::vec3> positions = {};
	std::vector<glm::quat> rotations = {};
	std::vector<glm::vec3> scales = {};
};

struct Animation
{
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
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
