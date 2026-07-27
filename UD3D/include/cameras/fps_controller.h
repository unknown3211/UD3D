#pragma once

#include <GLFW/glfw3.h>
#include "camera.h"
#include "physics/physics_system.h"
#include "utils/glm_includes.h"

class FPSController
{
public:
    FPSController();
    ~FPSController();

    void Init(PhysicsSystem* physics, const glm::vec3& position, float eyeHeight = 0.8f);
    void Update(GLFWwindow* window, float deltaTime);
    void Deactivate();

    Camera& GetCam() { return m_cam; }
    b3BodyId GetBodyId() const { return m_bodyId; }

    void SetMouseCapture(GLFWwindow* window, bool capture);
    bool IsMouseCaptured() const { return m_mouseCaptured; }

    float GetYawDelta() const { return m_yawDelta; }
    float GetPitchDelta() const { return m_pitchDelta; }

private:
    void ProcessInput(GLFWwindow* window, float deltaTime);
    void UpdateMouseLook(GLFWwindow* window);

    Camera m_cam;
    PhysicsSystem* m_physics = nullptr;
    b3BodyId m_bodyId = b3_nullBodyId;

    glm::vec3 m_halfExtents = glm::vec3(0.4f, 0.9f, 0.4f);
    float m_eyeHeight = 0.8f;

    float m_walkSpeed = 3.0f;
    float m_sprintSpeed = 8.0f;
    float m_groundAccel = 14.0f;
    float m_friction = 8.0f;
    float m_airAccel = 35.0f;
    float m_airCap = 3.0f;
    float m_jumpForce = 7.2f;
    float m_sensitivity = 0.1f;

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;
    float m_yawDelta = 0.0f;
    float m_pitchDelta = 0.0f;

    bool m_active = false;
    bool m_mouseCaptured = true;
    bool m_firstMouse = true;
    double m_lastX = 0.0;
    double m_lastY = 0.0;

    bool m_tabPressedLastFrame = false;
};
