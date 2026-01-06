#include "cameras/camera.h"
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 Camera::worldUp{ 0.0f, 1.0f, 0.0f };

Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov, float aspect)
    : position(position), Yaw(yaw), Pitch(pitch), FOV(fov), Aspect(aspect), forward(), right(), up()
{
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(FOV), Aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}

void Camera::UpdateDirection()
{
    forward = glm::normalize(glm::vec3(
        glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch)),
        glm::sin(glm::radians(Pitch)),
        glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch))
    ));

    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
}