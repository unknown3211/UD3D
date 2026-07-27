#include "player.h"
#include "logs.h"

Player::Player() {}

Player::~Player() 
{
    Shutdown();
}

void Player::Init(PhysicsSystem* physics) 
{
    m_fpsController.Init(physics, glm::vec3(0.0f, 2.0f, 6.0f), 0.8f);

    std::string modelPath = std::string(RESOURCES_PATH) + "models/ak.glb";
    std::string vertPath  = std::string(RESOURCES_PATH) + "shaders/default.vert";
    std::string fragPath  = std::string(RESOURCES_PATH) + "shaders/default.frag";

    Camera* cam = &m_fpsController.GetCam();
    m_akModel.LoadFromFile(modelPath, vertPath.c_str(), fragPath.c_str(), "", GL_RGBA, cam, glm::vec3(0.0f), glm::vec3(1.0f));

    if (m_akModel.HasAnimations()) 
    {
        Log(info, "AK Model GLB animations loaded successfully!");
    }
}

void Player::Update(PhysicsSystem* physics, GLFWwindow* window, float deltaTime) 
{
    m_fpsController.Update(window, deltaTime);
    HandleWeaponInput(window, deltaTime);
    UpdateViewmodel(physics, window, deltaTime);
}

void Player::HandleWeaponInput(GLFWwindow* window, float deltaTime) 
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        m_akRecoilTimer = 0.12f;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && m_akModel.HasAnimations()) {
        m_akCurrentAnim++;
        m_akModel.PlayAnimation(m_akCurrentAnim);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && m_akModel.HasAnimations()) {
        m_akCurrentAnim = 0;
        m_akModel.PlayAnimation(0);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && m_akModel.HasAnimations()) {
        m_akCurrentAnim = 1;
        m_akModel.PlayAnimation(1);
    }
}

void Player::UpdateViewmodel(PhysicsSystem* physics, GLFWwindow* window, float deltaTime) 
{
    if (m_akRecoilTimer > 0.0f) 
    {
        m_akRecoilTimer -= deltaTime;
    }

    Camera* cam = &m_fpsController.GetCam();

    static glm::vec3 smoothVel = glm::vec3(0.0f);
    glm::vec3 playerVel = physics ? physics->GetLinearVelocity(m_fpsController.GetBodyId()) : glm::vec3(0.0f);

    glm::vec3 horizVel  = glm::vec3(playerVel.x, 0.0f, playerVel.z);
    float springAlpha = std::min(1.0f, deltaTime * 8.0f);
    smoothVel = glm::mix(smoothVel, horizVel, springAlpha);

    glm::vec3 camRight   = cam->GetRight();
    glm::vec3 camForward = cam->GetForward();
    float localRight   = glm::dot(smoothVel, camRight);
    float localForward = glm::dot(smoothVel, camForward);

    float bobX = -localRight   * 0.00035f;
    float bobY = -localForward * 0.00018f;
    float strafeRoll = localRight * 0.18f;

    static float currentSwayX = 0.0f;
    static float currentSwayY = 0.0f;
    float targetSwayX = -m_fpsController.GetYawDelta()   * 0.001f;
    float targetSwayY =  m_fpsController.GetPitchDelta() * 0.001f;
    currentSwayX = glm::mix(currentSwayX, targetSwayX, std::min(1.0f, deltaTime * 14.0f));
    currentSwayY = glm::mix(currentSwayY, targetSwayY, std::min(1.0f, deltaTime * 14.0f));

    m_akBobTimer = 0.0f;

    float recoilKick  = (m_akRecoilTimer > 0.0f) ? (m_akRecoilTimer * 0.35f) : 0.0f;
    float recoilPitch = (m_akRecoilTimer > 0.0f) ? (m_akRecoilTimer * 22.0f) : 0.0f;

    float finalX = m_baseOffsetX + bobX + currentSwayX;
    float finalY = m_baseOffsetY + bobY + currentSwayY + (m_akRecoilTimer * 0.08f);
    float finalZ = m_baseOffsetZ - recoilKick;

    int winW = 1280, winH = 720;
    glfwGetFramebufferSize(window, &winW, &winH);
    float aspect = (winH > 0) ? (float)winW / (float)winH : (4.0f / 3.0f);
    glm::mat4 vmProj = glm::perspective(glm::radians(68.0f), aspect, 0.01f, 10.0f);
    glm::mat4 vmView = glm::mat4(1.0f);

    glm::vec3 vmRight   = glm::vec3(1.0f, 0.0f,  0.0f);
    glm::vec3 vmUp      = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::vec3 vmForward = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 vmPos = (vmRight * finalX) + (vmUp * finalY) + (vmForward * finalZ);

    glm::mat4 vmRot = glm::rotate(glm::mat4(1.0f), glm::radians(m_baseRotY), glm::vec3(0.0f, 1.0f, 0.0f));
    vmRot = glm::rotate(vmRot, glm::radians(m_baseRotX), glm::vec3(1.0f, 0.0f, 0.0f));
    vmRot = glm::rotate(vmRot, glm::radians(m_baseRotZ + strafeRoll), glm::vec3(0.0f, 0.0f, 1.0f));
    if (recoilPitch > 0.0f) 
    {
        vmRot = glm::rotate(vmRot, glm::radians(recoilPitch), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::mat4 vmScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleVal));
    glm::mat4 weaponTransform = glm::translate(glm::mat4(1.0f), vmPos) * vmRot * vmScale;

    m_akModel.SetTransformMatrix(weaponTransform);
    m_akModel.SetViewMatrixOverride(vmView);
    m_akModel.SetProjectionOverride(vmProj);
}

void Player::Draw(Lighting& light) 
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_akModel.Draw(light);
}

void Player::Shutdown() 
{
    m_akModel.Shutdown();
}