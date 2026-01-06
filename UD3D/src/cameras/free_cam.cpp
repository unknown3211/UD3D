#include <GLFW/glfw3.h>
#include "cameras/free_cam.h"

FCamera::FCamera() :
    cam(glm::vec3(0.0f), -90.0f, -20.0f),
    speed(1.0f),
    baseSpeed(1.0f),
    yaw(-90.0f),
    pitch(-20.0f),
    active(false),
    IsFirstMousePressed(true),
    lastX(0.0),
    lastY(0.0),
    camPos(glm::vec3(0.0f))
{
}

void FCamera::Activate(const glm::vec3& position)
{
    camPos = position;
    cam = Camera(position, -90.0f, -20.0f);
    speed = 1.0f;
    baseSpeed = speed;
    yaw = -90.0f;
    pitch = -20.0f;
    active = true;
    IsFirstMousePressed = true;
    lastX = lastY = 0.0;
}

void FCamera::Update(GLFWwindow* window, float deltaTime)
{
    if (!active) return;
    Keybinds(window, deltaTime);
    cam.UpdateDirection();
}

void FCamera::Deactivate()
{
    active = false;
}

void FCamera::Keybinds(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.GetPosition() += cam.GetForward() * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.GetPosition() -= cam.GetForward() * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.GetPosition() -= cam.GetRight() * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.GetPosition() += cam.GetRight() * speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = baseSpeed + 6.0f;
    }
    else
    {
        speed = baseSpeed;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (IsFirstMousePressed)
        {
            lastX = xpos;
            lastY = ypos;
            IsFirstMousePressed = false;
        }

        float xoffset = (float)(xpos - lastX) * sensitivity;
        float yoffset = (float)(lastY - ypos) * sensitivity;

        lastX = xpos;
        lastY = ypos;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)  pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        cam.SetYawPitch(yaw, pitch);
        cam.UpdateDirection();
    }
    else
    {
        IsFirstMousePressed = true;
    }
}