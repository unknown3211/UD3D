#pragma once

#include <GLFW/glfw3.h>
#include "camera.h"
#include "utils/glm_includes.h"

class FCamera
{
public:
    FCamera();
    void Activate(const glm::vec3& position);
    void Deactivate();
    void Update(GLFWwindow* window, float deltaTime);
    Camera& GetCam() { return cam; }
private:
    void Keybinds(GLFWwindow* window, float deltaTime);
    Camera cam;
    bool active = false;
    float speed;
    float baseSpeed;
    float sensitivity = 0.05f;
    float yaw;
    float pitch;
    glm::vec3 camPos;
    bool IsFirstMousePressed = true;
    double lastX = 0.0, lastY = 0.0;
};