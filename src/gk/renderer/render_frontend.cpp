#include "renderer.h"
#include "adapter.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gk/model/prefabs.h"


using namespace juye;


//======================================================================
// Temporary frontend while we build out driver backend
//======================================================================

struct camera{
  glm::mat4 view;
  glm::mat4 projection;
};

struct scene{
  camera camera;

  //objects
  juye::Prefab cube_default;
  juye::Prefab cube_texture;
  juye::Prefab plane;

  scene(){
    create_scene_objects();
  }

private:
  void create_scene_skybox(){

  }

  void create_scene_objects(){
      cube_default = Prefab::Builder().set_mesh(Prefab::PrefabMeshCube).build();
      create_scene_camera();
  }

  void create_scene_camera(){
    camera.view = glm::mat4(1);
    camera.projection = glm::mat4(1);
    camera.view = glm::lookAt(glm::vec3(0.0f, 3.0f, -8.0f), glm::vec3(0.0f, 2.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    camera.projection = glm::perspectiveFov(glm::radians(60.0f), static_cast<float>(3000), static_cast<float>(2000), 0.1f, 1000.0f);
  }

};

struct gk_draw_data{
  gdi_memory indices;
  gdi_memory vertices;
  size_t transform_offset;
};

//======================================================================
//======================================================================


gdi_device driver{};
scene scene{};


namespace juye{

gdi_memory index;
gdi_memory vertex;
gdi_memory transform;

void render_fe_begin(){
  driver.init_driver();
  index = driver.allocate_memory(nullptr, scene.cube_default.indice_bytes, GDI_MEMORY_LINEAR_BIT);
  vertex = driver.allocate_memory(nullptr, scene.cube_default.vertice_bytes, GDI_MEMORY_LINEAR_BIT);
  // transform = driver.allocate_memory(nullptr, bytes, GDI_MEMORY_LINEAR_BIT);
  driver.write_memory(scene.cube_default.vertices, vertex, scene.cube_default.vertice_bytes, 0);
  driver.write_memory(scene.cube_default.indices, index, scene.cube_default.indice_bytes, 0);
  
}

void render_fe_tick(){
  float* p = glm::value_ptr(scene.camera.projection);
  float* v = glm::value_ptr(scene.camera.view);
  driver.set_projection(v, p);
  

  driver.dummy_draw(vertex, index, scene.cube_default.n_indices);
  driver.submit_frame();
}

void render_fe_end(){

}

}
