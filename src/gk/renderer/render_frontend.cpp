#include "renderer.h"
#include "adapter.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "gk/model/prefabs.h"

struct Camera{
  glm::mat4 view;
  glm::mat4 projection;
};

struct Scene{
  Camera camera;

  //objects
  juye::Prefab cube_default;
  juye::Prefab cube_texture;
  juye::Prefab plane;
};


Scene scene;

namespace juye{

void create_scene_skybox(){

}

void create_scene_objects(){
    scene.cube_default = juye::Prefab::Builder().set_mesh(juye::Prefab::PrefabMeshCube).build();
}

void create_scene_camera(){
  scene.camera.view = glm::lookAt(glm::vec3(0.0f, -4.0f, -4.0f), glm::vec3(0.0f, 1.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  scene.camera.projection = glm::perspectiveFov(glm::radians(60.0f), static_cast<float>(1424), static_cast<float>(714), 0.1f, 1000.0f);
}

void render_fe_begin(){
  create_scene_objects();
  create_scene_camera();
  create_scene_skybox();
}

void render_fe_tick(){

}

void render_fe_end(){}

}
