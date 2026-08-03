#include "renderer.h"
#include "adapter.h"
#include "gk/model/prefabs.h"
#include "fsystem/file.h"
#include "base/global.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



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
    camera.view = glm::lookAt(glm::vec3(0.0f, 1.0f, -4.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    camera.projection = glm::perspectiveFov(glm::radians(60.0f), static_cast<float>(3000), static_cast<float>(2000), 0.1f, 1000.0f);
  }

};

struct gk_draw_data{
  gdi_memory ibo;
  gdi_memory vbo;
  gdi_memory texture;
  size_t n_indices;
};

//======================================================================
//======================================================================


//======================================================================
// Temporary input system
//======================================================================
//
//
//======================================================================
//======================================================================
//

namespace juye{

gdi_device driver{};
scene scene{};
std::vector<gk_draw_data> drawlist;

gdi_transform t1;
glm::mat4 m1;
gdi_memory cbuf;

struct gk_constant_buffer{
  uint32_t transform_offset;
};

gdi_rpool res_pool;

gk_draw_data construct_drawable(const Prefab& fab){
  gk_draw_data ret{};
  res_pool  = driver.allocate_resource_pool();

  ret.n_indices = fab.n_indices;
  ret.ibo = driver.allocate_buffer(res_pool, fab.indice_bytes, 0);
  driver.write_buffer(fab.indices, ret.ibo, fab.indice_bytes, 0);

  ret.vbo = driver.allocate_buffer(res_pool, fab.vertice_bytes, 0);
  driver.write_buffer(fab.vertices, ret.vbo, fab.vertice_bytes, 0);

  auto n = gk_constant_buffer{0};

  cbuf = driver.allocate_buffer(res_pool, sizeof(gk_constant_buffer), 0);
  driver.write_buffer(&n, cbuf, sizeof(gk_constant_buffer), 0);

  fs_image_data image = fs_load_image("juye/data/textures/nx.png");

  if(!image.data){
    _juye_crashf("failed to load image");
  }


  ret.texture = driver.allocate_texture(res_pool, image.width, image.height, 1);
  driver.write_texture(image.data, ret.texture, image.width, image.width, 0);
  fs_unload_image(image);

  driver.commit_resource_pool(res_pool);
  return ret;
}

void render_fe_begin(){
  driver.init_driver();
  drawlist.push_back(construct_drawable(scene.cube_default));
  m1 = glm::mat4(1);
  t1 = driver.generate_transform();
}

void render_fe_tick(){
  float* p = glm::value_ptr(scene.camera.projection);
  float* v = glm::value_ptr(scene.camera.view);
  driver.set_projection(v, p);
  
  glm::mat4 a = glm::rotate(m1, 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));
  m1 = a;
  driver.write_transform(t1, glm::value_ptr(m1));
  gk_draw_data d = drawlist[0];
  driver.dummy_draw(d.vbo, d.ibo, d.n_indices, t1, cbuf, d.texture, res_pool);
  driver.submit_frame();
  
}

void render_fe_end(){

}

}
