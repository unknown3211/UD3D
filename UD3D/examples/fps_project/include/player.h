#pragma once

#include "engine.h"

class Player {
public:
    Player();
    ~Player();

    void Init(PhysicsSystem* physics);
    void Update(PhysicsSystem* physics, GLFWwindow* window, float deltaTime);
    void Draw(Lighting& light);
    void Shutdown();

    Camera& GetCam() { return m_fpsController.GetCam(); }
    FPSController& GetFPSController() { return m_fpsController; }

private:
    void HandleWeaponInput(GLFWwindow* window, float deltaTime);
    void UpdateViewmodel(PhysicsSystem* physics, GLFWwindow* window, float deltaTime);

    FPSController m_fpsController;
    Model m_akModel;

    float m_akBobTimer = 0.0f;
    float m_akRecoilTimer = 0.0f;
    int m_akCurrentAnim = 0;

    float m_scaleVal = 0.2330f;
    float m_baseOffsetX = 0.0230f;
    float m_baseOffsetY = -0.0610f;
    float m_baseOffsetZ = 0.0500f;
    float m_baseRotX = -0.00f;
    float m_baseRotY = -7.63f;
    float m_baseRotZ = 0.00f;
};
