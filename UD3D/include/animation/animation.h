#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <assimp/scene.h>
#include "utils/glm_includes.h"
#include <glm/gtc/quaternion.hpp>

struct BoneInfo
{
	int id = 0;
	glm::mat4 offset = glm::mat4(1.0f);
};

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class BoneChannel
{
public:
	BoneChannel(const std::string& name, int id, const aiNodeAnim* channel);

	void Update(float animationTime);

	glm::mat4 GetLocalTransform() const { return m_localTransform; }
	std::string GetBoneName() const { return m_name; }
	int GetBoneID() const { return m_id; }

	int GetPositionIndex(float animationTime) const;
	int GetRotationIndex(float animationTime) const;
	int GetScaleIndex(float animationTime) const;

private:
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const;
	glm::mat4 InterpolatePosition(float animationTime) const;
	glm::mat4 InterpolateRotation(float animationTime) const;
	glm::mat4 InterpolateScaling(float animationTime) const;

	std::vector<KeyPosition> m_positions;
	std::vector<KeyRotation> m_rotations;
	std::vector<KeyScale> m_scales;
	int m_numPositions = 0;
	int m_numRotations = 0;
	int m_numScales = 0;

	glm::mat4 m_localTransform = glm::mat4(1.0f);
	std::string m_name;
	int m_id = -1;
};

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount = 0;
	std::vector<AssimpNodeData> children;
};

class AnimationClip
{
public:
	AnimationClip() = default;
	AnimationClip(const std::string& animationPath, const aiScene* scene, int animationIndex, std::unordered_map<std::string, BoneInfo>& boneInfoMap, int& boneCount);

	BoneChannel* FindBoneChannel(const std::string& name);

	float GetTicksPerSecond() const { return m_ticksPerSecond; }
	float GetDuration() const { return m_duration; }
	const AssimpNodeData& GetRootNode() const { return m_rootNode; }
	const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap() const { return m_boneInfoMap; }

private:
	void ReadMissingBones(const aiNodeAnim* channel, std::unordered_map<std::string, BoneInfo>& boneInfoMap, int& boneCount);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

	float m_duration = 0.0f;
	float m_ticksPerSecond = 1.0f;
	std::vector<BoneChannel> m_bones;
	AssimpNodeData m_rootNode;
	std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
};
