#include "animation/animator.h"
#include "animation/animation.h"
#include "logs.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

static glm::vec3 GetGLMVec(const aiVector3D& vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

static glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
{
	return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
}

BoneChannel::BoneChannel(const std::string& name, int id, const aiNodeAnim* channel)
	: m_name(name), m_id(id), m_localTransform(1.0f)
{
	m_numPositions = channel->mNumPositionKeys;
	for (int positionIndex = 0; positionIndex < m_numPositions; ++positionIndex)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
		KeyPosition data;
		data.position = GetGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		m_positions.push_back(data);
	}

	m_numRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < m_numRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);
		KeyRotation data;
		data.orientation = GetGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		m_rotations.push_back(data);
	}

	m_numScales = channel->mNumScalingKeys;
	for (int keyIndex = 0; keyIndex < m_numScales; ++keyIndex)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = static_cast<float>(channel->mScalingKeys[keyIndex].mTime);
		KeyScale data;
		data.scale = GetGLMVec(scale);
		data.timeStamp = timeStamp;
		m_scales.push_back(data);
	}
}

int BoneChannel::GetPositionIndex(float animationTime) const
{
	for (int index = 0; index < m_numPositions - 1; ++index)
	{
		if (animationTime < m_positions[index + 1].timeStamp)
			return index;
	}
	return 0;
}

int BoneChannel::GetRotationIndex(float animationTime) const
{
	for (int index = 0; index < m_numRotations - 1; ++index)
	{
		if (animationTime < m_rotations[index + 1].timeStamp)
			return index;
	}
	return 0;
}

int BoneChannel::GetScaleIndex(float animationTime) const
{
	for (int index = 0; index < m_numScales - 1; ++index)
	{
		if (animationTime < m_scales[index + 1].timeStamp)
			return index;
	}
	return 0;
}

float BoneChannel::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const
{
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	if (framesDiff <= 0.0001f) return 0.0f;
	return midWayLength / framesDiff;
}

glm::mat4 BoneChannel::InterpolatePosition(float animationTime) const
{
	if (1 == m_numPositions)
		return glm::translate(glm::mat4(1.0f), m_positions[0].position);

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_positions[p0Index].timeStamp, m_positions[p1Index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(m_positions[p0Index].position, m_positions[p1Index].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 BoneChannel::InterpolateRotation(float animationTime) const
{
	if (1 == m_numRotations)
	{
		auto rotation = glm::normalize(m_rotations[0].orientation);
		return glm::mat4_cast(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_rotations[p0Index].timeStamp, m_rotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(m_rotations[p0Index].orientation, m_rotations[p1Index].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::mat4_cast(finalRotation);
}

glm::mat4 BoneChannel::InterpolateScaling(float animationTime) const
{
	if (1 == m_numScales)
		return glm::scale(glm::mat4(1.0f), m_scales[0].scale);

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_scales[p0Index].timeStamp, m_scales[p1Index].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(m_scales[p0Index].scale, m_scales[p1Index].scale, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}

void BoneChannel::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScaling(animationTime);
	m_localTransform = translation * rotation * scale;
}

AnimationClip::AnimationClip(const std::string& animationPath, const aiScene* scene, int animationIndex, std::unordered_map<std::string, BoneInfo>& boneInfoMap, int& boneCount)
{
	if (!scene || !scene->mAnimations || animationIndex >= static_cast<int>(scene->mNumAnimations))
	{
		Log(warn, "Invalid animation scene or index");
		return;
	}

	aiAnimation* animation = scene->mAnimations[animationIndex];
	m_duration = static_cast<float>(animation->mDuration);
	m_ticksPerSecond = animation->mTicksPerSecond != 0 ? static_cast<float>(animation->mTicksPerSecond) : 25.0f;

	ReadHierarchyData(m_rootNode, scene->mRootNode);

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.C_Str();

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_bones.push_back(BoneChannel(channel->mNodeName.C_Str(), boneInfoMap[channel->mNodeName.C_Str()].id, channel));
	}

	m_boneInfoMap = boneInfoMap;
}

BoneChannel* AnimationClip::FindBoneChannel(const std::string& name)
{
	for (auto& bone : m_bones)
	{
		if (bone.GetBoneName() == name)
			return &bone;
	}
	return nullptr;
}

void AnimationClip::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
	if (!src) return;

	dest.name = src->mName.data;
	dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (unsigned int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

Animator::Animator(AnimationClip* currentAnimation)
{
	m_currentTime = 0.0f;
	m_currentAnimation = currentAnimation;

	m_finalBoneMatrices.reserve(MAX_BONES);
	for (int i = 0; i < MAX_BONES; i++)
		m_finalBoneMatrices.push_back(glm::mat4(1.0f));

	if (m_currentAnimation)
	{
		CalculateBoneTransform(&m_currentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::PlayAnimation(AnimationClip* pAnimation)
{
	m_currentAnimation = pAnimation;
	m_currentTime = 0.0f;
	if (m_currentAnimation)
	{
		CalculateBoneTransform(&m_currentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::UpdateAnimation(float dt)
{
	m_deltaTime = dt;
	if (m_currentAnimation)
	{
		m_currentTime += m_currentAnimation->GetTicksPerSecond() * dt;
		m_currentTime = fmod(m_currentTime, m_currentAnimation->GetDuration());
		CalculateBoneTransform(&m_currentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	BoneChannel* bone = m_currentAnimation->FindBoneChannel(nodeName);

	if (bone)
	{
		bone->Update(m_currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_currentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		if (index < MAX_BONES)
		{
			if (index >= static_cast<int>(m_finalBoneMatrices.size()))
				m_finalBoneMatrices.resize(index + 1, glm::mat4(1.0f));
			m_finalBoneMatrices[index] = globalTransformation * offset;
		}
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}
