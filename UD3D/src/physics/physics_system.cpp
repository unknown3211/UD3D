#include "physics/physics_system.h"
#include "logs.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
	Shutdown();
}

void PhysicsSystem::Init(glm::vec3 gravity)
{
	if (b3World_IsValid(m_worldId))
	{
		Shutdown();
	}

	b3WorldDef worldDef = b3DefaultWorldDef();
	worldDef.gravity = { gravity.x, gravity.y, gravity.z };
	m_worldId = b3CreateWorld(&worldDef);

	if (b3World_IsValid(m_worldId))
	{
		Log(info, "Box3D Physics System Initialized successfully.");
	}
	else
	{
		Log(error, "Failed to initialize Box3D Physics System!");
	}
}

void PhysicsSystem::Shutdown()
{
	if (b3World_IsValid(m_worldId))
	{
		b3DestroyWorld(m_worldId);
		m_worldId = b3_nullWorldId;
		Log(info, "Box3D Physics System Shutdown.");
	}
}

void PhysicsSystem::Step(float deltaTime, int subStepCount)
{
	if (!b3World_IsValid(m_worldId)) return;

	if (deltaTime > 0.1f) deltaTime = 0.1f;
	b3World_Step(m_worldId, deltaTime, subStepCount);
}

b3BodyId PhysicsSystem::CreateBoxBody(glm::vec3 position, glm::vec3 halfExtents, bool isDynamic, float density, float friction, float restitution)
{
	if (!b3World_IsValid(m_worldId)) return b3_nullBodyId;

	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = isDynamic ? b3_dynamicBody : b3_staticBody;
	bodyDef.position = { position.x, position.y, position.z };

	b3BodyId bodyId = b3CreateBody(m_worldId, &bodyDef);
	if (!b3Body_IsValid(bodyId)) return b3_nullBodyId;

	b3ShapeDef shapeDef = b3DefaultShapeDef();
	shapeDef.density = density;
	shapeDef.baseMaterial.friction = friction;
	shapeDef.baseMaterial.restitution = restitution;

	b3BoxHull boxHull = b3MakeBoxHull(halfExtents.x, halfExtents.y, halfExtents.z);
	b3CreateHullShape(bodyId, &shapeDef, &boxHull.base);

	return bodyId;
}

b3BodyId PhysicsSystem::CreateSphereBody(glm::vec3 position, float radius, bool isDynamic, float density, float friction, float restitution)
{
	if (!b3World_IsValid(m_worldId)) return b3_nullBodyId;

	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = isDynamic ? b3_dynamicBody : b3_staticBody;
	bodyDef.position = { position.x, position.y, position.z };

	b3BodyId bodyId = b3CreateBody(m_worldId, &bodyDef);
	if (!b3Body_IsValid(bodyId)) return b3_nullBodyId;

	b3ShapeDef shapeDef = b3DefaultShapeDef();
	shapeDef.density = density;
	shapeDef.baseMaterial.friction = friction;
	shapeDef.baseMaterial.restitution = restitution;

	b3Sphere sphere = { { 0.0f, 0.0f, 0.0f }, radius };
	b3CreateSphereShape(bodyId, &shapeDef, &sphere);

	return bodyId;
}

glm::vec3 PhysicsSystem::GetBodyPosition(b3BodyId bodyId) const
{
	if (!b3Body_IsValid(bodyId)) return glm::vec3(0.0f);
	b3Pos pos = b3Body_GetPosition(bodyId);
	return glm::vec3(pos.x, pos.y, pos.z);
}

glm::quat PhysicsSystem::GetBodyRotation(b3BodyId bodyId) const
{
	if (!b3Body_IsValid(bodyId)) return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	b3Quat q = b3Body_GetRotation(bodyId);
	return glm::quat(q.s, q.v.x, q.v.y, q.v.z);
}

glm::mat4 PhysicsSystem::GetBodyTransformMatrix(b3BodyId bodyId, glm::vec3 scale) const
{
	glm::vec3 pos = GetBodyPosition(bodyId);
	glm::quat rot = GetBodyRotation(bodyId);

	glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 rotationMat = glm::mat4_cast(rot);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

	return translationMat * rotationMat * scaleMat;
}

b3BodyId PhysicsSystem::CreateCharacterBody(glm::vec3 position, glm::vec3 halfExtents, float density, float friction)
{
	if (!b3World_IsValid(m_worldId)) return b3_nullBodyId;

	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = b3_dynamicBody;
	bodyDef.position = { position.x, position.y, position.z };
	bodyDef.motionLocks.angularX = true;
	bodyDef.motionLocks.angularY = true;
	bodyDef.motionLocks.angularZ = true;
	bodyDef.enableSleep = false;

	b3BodyId bodyId = b3CreateBody(m_worldId, &bodyDef);
	if (!b3Body_IsValid(bodyId)) return b3_nullBodyId;

	b3ShapeDef shapeDef = b3DefaultShapeDef();
	shapeDef.density = density;
	shapeDef.baseMaterial.friction = friction;
	shapeDef.baseMaterial.restitution = 0.0f;

	b3BoxHull boxHull = b3MakeBoxHull(halfExtents.x, halfExtents.y, halfExtents.z);
	b3CreateHullShape(bodyId, &shapeDef, &boxHull.base);

	return bodyId;
}

void PhysicsSystem::ApplyLinearImpulse(b3BodyId bodyId, glm::vec3 impulse, glm::vec3 point)
{
	if (!b3Body_IsValid(bodyId)) return;
	b3Vec3 bImpulse = { impulse.x, impulse.y, impulse.z };
	b3Pos bPoint = { point.x, point.y, point.z };
	b3Body_ApplyLinearImpulse(bodyId, bImpulse, bPoint, true);
}

void PhysicsSystem::SetLinearVelocity(b3BodyId bodyId, glm::vec3 velocity)
{
	if (!b3Body_IsValid(bodyId)) return;
	b3Vec3 bVel = { velocity.x, velocity.y, velocity.z };
	b3Body_SetLinearVelocity(bodyId, bVel);
}

glm::vec3 PhysicsSystem::GetLinearVelocity(b3BodyId bodyId) const
{
	if (!b3Body_IsValid(bodyId)) return glm::vec3(0.0f);
	b3Vec3 vel = b3Body_GetLinearVelocity(bodyId);
	return glm::vec3(vel.x, vel.y, vel.z);
}
