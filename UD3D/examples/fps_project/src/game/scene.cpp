#include "scene.h"
#include "logs.h"
#include <string>

Scene::Scene(Camera *camera, bool debugmode) 
    : debug(debugmode), cam(*camera) 
{
  if (debug) 
  {
    scene_mngr.debugMode(true);
  }

  OnInit(camera);
}

Scene::~Scene() 
{
  player.Shutdown();
  scene_mngr.Shutdown();

  for (auto &obj : physicsObjects) {
    delete obj.mesh;
  }

  physicsObjects.clear();
  physics.Shutdown();

  delete light;
  light = nullptr;
}

void Scene::OnInit(Camera *camera) 
{
  physics.Init(glm::vec3(0.0f, -16.0f, 0.0f));
  player.Init(&physics);

  Camera *activeCam = &player.GetCam();
  InitPhysics(activeCam);

  glm::vec3 groundPos = glm::vec3(-1.0f, 0.0f, 0.0f);
  glm::vec3 lightPos  = glm::vec3(1.5f, 2.0f, 0.0f);

  LoadGround(groundPos, activeCam);
  LoadLight(lightPos, activeCam);
  LoadText();
}

void Scene::InitPhysics(Camera *camera) 
{
  groundBody = physics.CreateBoxBody(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(10.0f, 0.5f, 10.0f), false);

  float s = 0.5f;
  std::vector<Vertex> cubeVerts = {
      {{-s, -s,  s}, {1.0f, 0.2f, 0.2f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
      {{ s, -s,  s}, {1.0f, 0.2f, 0.2f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
      {{ s,  s,  s}, {1.0f, 0.2f, 0.2f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
      {{-s,  s,  s}, {1.0f, 0.2f, 0.2f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

      {{-s, -s, -s}, {0.2f, 0.8f, 0.2f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
      {{-s,  s, -s}, {0.2f, 0.8f, 0.2f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
      {{ s,  s, -s}, {0.2f, 0.8f, 0.2f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
      {{ s, -s, -s}, {0.2f, 0.8f, 0.2f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},

      {{-s, s, -s}, {0.2f, 0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
      {{-s, s,  s}, {0.2f, 0.2f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
      {{ s, s,  s}, {0.2f, 0.2f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
      {{ s, s, -s}, {0.2f, 0.2f, 1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

      {{-s, -s, -s}, {0.9f, 0.9f, 0.2f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
      {{ s, -s, -s}, {0.9f, 0.9f, 0.2f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
      {{ s, -s,  s}, {0.9f, 0.9f, 0.2f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
      {{-s, -s,  s}, {0.9f, 0.9f, 0.2f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},

      {{ s, -s, -s}, {1.0f, 0.5f, 0.2f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
      {{ s,  s, -s}, {1.0f, 0.5f, 0.2f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
      {{ s,  s,  s}, {1.0f, 0.5f, 0.2f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
      {{ s, -s,  s}, {1.0f, 0.5f, 0.2f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

      {{-s, -s, -s}, {0.7f, 0.2f, 0.8f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
      {{-s, -s,  s}, {0.7f, 0.2f, 0.8f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
      {{-s,  s,  s}, {0.7f, 0.2f, 0.8f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
      {{-s,  s, -s}, {0.7f, 0.2f, 0.8f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}}
  };

  std::vector<unsigned int> cubeIndices = {
       0,  1,  2,  0,  2,  3,
       7,  6,  5,  7,  5,  4,
       8,  9, 10,  8, 10, 11,
      12, 13, 14, 12, 14, 15,
      19, 18, 17, 19, 17, 16,
      20, 21, 22, 20, 22, 23
  };

  std::string vertPath = std::string(RESOURCES_PATH) + "shaders/default.vert";
  std::string fragPath = std::string(RESOURCES_PATH) + "shaders/default.frag";
  std::string texPath  = std::string(RESOURCES_PATH) + "textures/rustic-bricks.png";

  float spawnHeights[] = {4.0f, 6.5f, 9.0f, 11.5f};
  float xOffsets[]     = {-0.3f, 0.2f, -0.1f, 0.4f};

  Camera *activeCam = (camera ? camera : &player.GetCam());

  for (int i = 0; i < 4; ++i) 
  {
    glm::vec3 spawnPos(xOffsets[i], spawnHeights[i], 0.0f);
    b3BodyId body = physics.CreateBoxBody(spawnPos, glm::vec3(0.5f), true, 1.0f, 0.6f, 0.4f);

    Mesh *mesh = new Mesh(cubeVerts, cubeIndices, vertPath, fragPath, texPath, GL_RGB, activeCam, spawnPos, glm::vec3(1.0f));

    PhysicsObject obj;
    obj.bodyId = body;
    obj.mesh   = mesh;
    obj.scale  = glm::vec3(1.0f);
    physicsObjects.push_back(obj);
  }
}

void Scene::Draw(Window &window, Camera *camera, float deltaTime) 
{
  player.Update(&physics, window.GetGLFWwindow(), deltaTime);
  physics.Step(deltaTime);

  for (auto &obj : physicsObjects) 
  {
    if (obj.mesh && b3Body_IsValid(obj.bodyId)) 
    {
      glm::mat4 transform = physics.GetBodyTransformMatrix(obj.bodyId, obj.scale);
      obj.mesh->SetTransformMatrix(transform);
    }
  }

  scene_mngr.Update(deltaTime);
  scene_mngr.Draw(*light);

  for (auto &obj : physicsObjects) 
  {
    if (obj.mesh) 
    {
      obj.mesh->Draw(*light);
    }
  }

  if (light) 
  {
    player.Draw(*light);
  }

  DrawLoadedText(window.GetDetails().s_width, window.GetDetails().s_height);
}

void Scene::LoadGround(glm::vec3 position, Camera *camera) 
{
  float s = 5.0f;
  float c = 1.0f;

  std::vector<Vertex> vertices = {
      {{-s, 0.0f, -s}, {c, c, c}, {0.0f, 0.0f}},
      {{ s, 0.0f, -s}, {c, c, c}, {1.0f, 0.0f}},
      {{ s, 0.0f,  s}, {c, c, c}, {1.0f, 1.0f}},
      {{-s, 0.0f,  s}, {c, c, c}, {0.0f, 1.0f}},
  };

  std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

  std::string vertPath = std::string(RESOURCES_PATH) + "shaders/default.vert";
  std::string fragPath = std::string(RESOURCES_PATH) + "shaders/default.frag";
  std::string texPath  = std::string(RESOURCES_PATH) + "textures/grass.jpg";

  scene_mngr.AddMesh(new Mesh(vertices, indices, vertPath, fragPath, texPath, GL_RGB, camera, position));
}

void Scene::LoadLight(glm::vec3 position, Camera *camera) 
{
  lightVerts.clear();

  float c_size = 0.5f;
  float half   = c_size * 0.5f;

  float lightVertices[] = {
      -half, -half,  half,  half, -half,  half,  half,  half,  half,  half,  half,  half,
      -half,  half,  half, -half, -half,  half,

      -half, -half, -half, -half,  half, -half,  half,  half, -half,  half,  half,
      -half,  half, -half, -half, -half, -half, -half,

      -half,  half,  half, -half,  half, -half, -half, -half, -half, -half, -half,
      -half, -half, -half,  half, -half,  half,  half,

       half,  half,  half,  half, -half,  half,  half, -half, -half,  half, -half,
      -half,  half,  half, -half,  half,  half,  half,

      -half,  half, -half, -half,  half,  half,  half,  half,  half,  half,  half,  half,
       half,  half, -half, -half,  half, -half,

      -half, -half, -half,  half, -half, -half,  half, -half,  half,  half, -half,
       half, -half, -half,  half,

      -half, -half, -half
  };

  unsigned int lightIndices[] = {
       0,  1,  2,  3,  4,  5,
       6,  7,  8,  9, 10, 11,
      12, 13, 14, 15, 16, 17,
      18, 19, 20, 21, 22, 23,
      24, 25, 26, 27, 28, 29,
      30, 31, 32, 33, 34, 35
  };

  for (int i = 0; i < 36; i++) 
  {
    Vertex v{};
    v.position = glm::vec3(lightVertices[i * 3 + 0], lightVertices[i * 3 + 1], lightVertices[i * 3 + 2]);
    v.normal  = glm::vec3(0.0f, 0.0f, 1.0f);
    v.texture = glm::vec2(0.0f, 0.0f);
    v.color   = glm::vec3(1.0f);
    lightVerts.push_back(v);
  }

  glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);

  std::vector<unsigned int> lightIndicesVec(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(unsigned int));
  light = new Lighting(lightVerts, position, glm::vec3(1.0f), color, camera, RESOURCES_PATH "shaders/lights/lighting.vert", RESOURCES_PATH "shaders/lights/lighting.frag");

  std::string vertPath = std::string(RESOURCES_PATH) + "shaders/lights/lighting.vert";
  std::string fragPath = std::string(RESOURCES_PATH) + "shaders/lights/lighting.frag";
  scene_mngr.AddMesh(new Mesh(lightVerts, lightIndicesVec, vertPath, fragPath, "", GL_RGB, camera, position));
}

void Scene::LoadText() {
  std::string fontPath = std::string(RESOURCES_PATH) + "fonts/Roboto-Bold.ttf";
  std::string vertPath = std::string(RESOURCES_PATH) + "shaders/ui/ui_text.vert";
  std::string fragPath = std::string(RESOURCES_PATH) + "shaders/ui/ui_text.frag";

  uiText.initText(fontPath, 32);
  textShader.LoadShaderProgramFromFile(vertPath.c_str(), fragPath.c_str());
}

void Scene::DrawLoadedText(int s_width, int s_height) {
 /*std::string text = "UD 3D - Shooter Sandbox";
  float x = s_width / 2.0f - 250.0f;
  float y = s_height - 50.0f;
  glm::vec2 pos = glm::vec2(x, y);
  uiText.renderText(textShader, text, pos, 0.8f, glm::vec3(0.9f, 0.9f, 1.0f), s_width, s_height);*/

  float crosshairX = s_width / 2.0f - 8.0f;
  float crosshairY = s_height / 2.0f - 12.0f;
  uiText.renderText(textShader, "+", glm::vec2(crosshairX, crosshairY), 0.75f, glm::vec3(0.2f, 1.0f, 0.2f), s_width, s_height);
}