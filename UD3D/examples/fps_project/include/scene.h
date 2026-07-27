#pragma once

#include "engine.h"
#include "player.h"
#include "dev.h"
#include <vector>

struct PhysicsObject {
  b3BodyId bodyId;
  Mesh *mesh = nullptr;
  glm::vec3 scale = glm::vec3(1.0f);
};

class Scene {
public:
  Scene(Camera *camera, bool debugmode);
  ~Scene();
  void OnInit(Camera *camera);
  void Draw(Window &window, Camera *camera, float deltaTime);
  void LoadGround(glm::vec3 position, Camera *camera);
  void LoadLight(glm::vec3 position, Camera *camera);

  void LoadText();
  void DrawLoadedText(int s_width, int s_height);

  void InitPhysics(Camera *camera);

  FPSController &GetFPSController() { return player.GetFPSController(); }

private:
  SceneManager scene_mngr;
  Player player;
  Camera &cam;
  Dev dev;
  bool debug;
  
  Lighting *light = nullptr;
  std::vector<Vertex> lightVerts;

  UIText uiText;
  Shader textShader;

  PhysicsSystem physics;
  b3BodyId groundBody = b3_nullBodyId;
  std::vector<PhysicsObject> physicsObjects;
};
