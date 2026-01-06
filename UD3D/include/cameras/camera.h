#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f, float fov = 45.0f, float aspect = 4.0f / 3.0f);

    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();

    void SetAspectRatio(float aspect) { Aspect = aspect; }
    void SetPosition(const glm::vec3& pos) { position = pos; }
    void SetYawPitch(float newYaw, float newPitch) { Yaw = newYaw; Pitch = newPitch; }

    void UpdateDirection();

    glm::vec3 GetForward() { return forward; }
    glm::vec3 GetRight() { return right; }
    glm::vec3 GetUp() { return up; }
    glm::vec3& GetPosition() { return position; }

private:
    glm::vec3 position;
    float Yaw;
    float Pitch;
    float FOV;
    float Aspect;
    static const glm::vec3 worldUp;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
};