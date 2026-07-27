#pragma once

#include <box3d/box3d.h>
#include "utils/glm_includes.h"
#include <vector>

struct PhysicsBodyInfo
{
	b3BodyId bodyId;
	glm::vec3 halfExtents;
	bool isDynamic;
};

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void Init(glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f));
	void Shutdown();

	void Step(float deltaTime, int subStepCount = 4);

	b3BodyId CreateBoxBody(glm::vec3 position, glm::vec3 halfExtents, bool isDynamic = true, float density = 1.0f, float friction = 0.5f, float restitution = 0.3f);
	b3BodyId CreateSphereBody(glm::vec3 position, float radius, bool isDynamic = true, float density = 1.0f, float friction = 0.5f, float restitution = 0.5f);
	b3BodyId CreateCharacterBody(glm::vec3 position, glm::vec3 halfExtents, float density = 1.0f, float friction = 0.1f);

	glm::vec3 GetBodyPosition(b3BodyId bodyId) const;
	glm::quat GetBodyRotation(b3BodyId bodyId) const;
	glm::mat4 GetBodyTransformMatrix(b3BodyId bodyId, glm::vec3 scale = glm::vec3(1.0f)) const;

	void ApplyLinearImpulse(b3BodyId bodyId, glm::vec3 impulse, glm::vec3 point);
	void SetLinearVelocity(b3BodyId bodyId, glm::vec3 velocity);
	glm::vec3 GetLinearVelocity(b3BodyId bodyId) const;

	bool IsInitialized() const { return b3World_IsValid(m_worldId); }
	b3WorldId GetWorldId() const { return m_worldId; }

private:
	b3WorldId m_worldId = b3_nullWorldId;
};
