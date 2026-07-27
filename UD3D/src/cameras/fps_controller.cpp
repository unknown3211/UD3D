#include "cameras/fps_controller.h"
#include <iostream>

FPSController::FPSController()
    : m_cam(glm::vec3(0.0f), -90.0f, 0.0f), m_yaw(-90.0f), m_pitch(0.0f) {}

FPSController::~FPSController() { Deactivate(); }

void FPSController::Init(PhysicsSystem *physics, const glm::vec3 &position, float eyeHeight) 
{
  m_physics = physics;
  m_eyeHeight = eyeHeight;
  m_yaw = -90.0f;
  m_pitch = 0.0f;

  if (m_physics && m_physics->IsInitialized()) 
  {
    m_bodyId = m_physics->CreateCharacterBody(position, m_halfExtents, 1.0f, 0.1f);
  }

  glm::vec3 eyePos = position + glm::vec3(0.0f, m_eyeHeight, 0.0f);
  m_cam = Camera(eyePos, m_yaw, m_pitch);
  m_cam.UpdateDirection();

  m_active = true;
  m_firstMouse = true;
}

void FPSController::Deactivate() 
{
  m_active = false;
  m_bodyId = b3_nullBodyId;
  m_physics = nullptr;
}

void FPSController::SetMouseCapture(GLFWwindow *window, bool capture) 
{
  m_mouseCaptured = capture;
  if (window) 
  {
    if (capture) 
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      m_firstMouse = true;
    } 
    else 
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

void FPSController::Update(GLFWwindow *window, float deltaTime) 
{
  if (!m_active || !m_physics || !window)
    return;

  bool tabPressed = (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS);
  if (tabPressed && !m_tabPressedLastFrame) 
  {
    SetMouseCapture(window, !m_mouseCaptured);
  }

  m_tabPressedLastFrame = tabPressed;

  if (m_mouseCaptured) 
  {
    UpdateMouseLook(window);
  }

  ProcessInput(window, deltaTime);

  if (b3Body_IsValid(m_bodyId)) 
  {
    glm::vec3 bodyPos = m_physics->GetBodyPosition(m_bodyId);
    glm::vec3 eyePos = bodyPos + glm::vec3(0.0f, m_eyeHeight, 0.0f);
    m_cam.SetPosition(eyePos);
  }

  m_cam.SetYawPitch(m_yaw, m_pitch);
  m_cam.UpdateDirection();
}

void FPSController::UpdateMouseLook(GLFWwindow *window) 
{
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  if (m_firstMouse) 
  {
    m_lastX = xpos;
    m_lastY = ypos;
    m_firstMouse = false;
  }

  float xoffset = static_cast<float>(xpos - m_lastX) * m_sensitivity;
  float yoffset = static_cast<float>(m_lastY - ypos) * m_sensitivity;

  m_lastX = xpos;
  m_lastY = ypos;

  m_yawDelta = xoffset;
  m_pitchDelta = yoffset;

  m_yaw += xoffset;
  m_pitch += yoffset;

  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f)
    m_pitch = -89.0f;
}

void FPSController::ProcessInput(GLFWwindow *window, float deltaTime) 
{
  if (!b3Body_IsValid(m_bodyId))
    return;

  float radYaw = glm::radians(m_yaw);
  glm::vec3 forward = glm::normalize(glm::vec3(std::cos(radYaw), 0.0f, std::sin(radYaw)));
  glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

  glm::vec3 wishDir(0.0f);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    wishDir += forward;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    wishDir -= forward;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    wishDir -= right;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    wishDir += right;

  float targetMaxSpeed = m_walkSpeed;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
  {
    targetMaxSpeed = m_sprintSpeed;
  }

  glm::vec3 bodyPos = m_physics->GetBodyPosition(m_bodyId);
  glm::vec3 currentVel = m_physics->GetLinearVelocity(m_bodyId);

  glm::vec3 horizVel = glm::vec3(currentVel.x, 0.0f, currentVel.z);
  float vertVel = currentVel.y;

  bool isGrounded = (bodyPos.y <= 0.95f) || (std::abs(vertVel) < 0.25f && bodyPos.y <= 1.5f);

  if (isGrounded) 
  {
    float speed = glm::length(horizVel);
    if (speed > 0.01f) 
    {
      float drop = speed * m_friction * deltaTime;
      float newSpeed = std::max(0.0f, speed - drop);
      horizVel *= (newSpeed / speed);
    } 
    else 
    {
      horizVel = glm::vec3(0.0f);
    }

    if (glm::length(wishDir) > 0.001f) 
    {
      wishDir = glm::normalize(wishDir);
      float currentSpeedInWishDir = glm::dot(horizVel, wishDir);
      float addSpeed = targetMaxSpeed - currentSpeedInWishDir;
      if (addSpeed > 0.0f) {
        float accelSpeed = m_groundAccel * targetMaxSpeed * deltaTime;
        accelSpeed = std::min(accelSpeed, addSpeed);
        horizVel += wishDir * accelSpeed;
      }
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
    {
      vertVel = m_jumpForce;
      isGrounded = false;
    }
  } 
  else 
  {
    if (glm::length(wishDir) > 0.001f) 
    {
      wishDir = glm::normalize(wishDir);
      float currentSpeedInWishDir = glm::dot(horizVel, wishDir);
      float addSpeed = m_airCap - currentSpeedInWishDir;
      if (addSpeed > 0.0f) {
        float accelSpeed = m_airAccel * targetMaxSpeed * deltaTime;
        accelSpeed = std::min(accelSpeed, addSpeed);
        horizVel += wishDir * accelSpeed;
      }
    }
  }

  glm::vec3 finalVel = glm::vec3(horizVel.x, vertVel, horizVel.z);
  m_physics->SetLinearVelocity(m_bodyId, finalVel);
}
