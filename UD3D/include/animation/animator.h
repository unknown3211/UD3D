#pragma once

#include "animation.h"
#include <vector>
#include <map>
#include "utils/glm_includes.h"

const int MAX_BONES = 100;

class Animator
{
public:
	Animator() = default;
	Animator(AnimationClip* currentAnimation);

	void UpdateAnimation(float dt);
	void PlayAnimation(AnimationClip* pAnimation);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices() const { return m_finalBoneMatrices; }

private:
	std::vector<glm::mat4> m_finalBoneMatrices;
	AnimationClip* m_currentAnimation = nullptr;
	float m_currentTime = 0.0f;
	float m_deltaTime = 0.0f;
};
