#include "renderer.h"
#include "drivers/gkhi/interface.h"
#include "fsystem/file.h"
#include "graphick/model/prefabs.h"

#include "base/global.h"
#include "base/memory.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bk/memory/bkmemory.h"



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
  gdi_resource texture;
  gdi_resource indices;
  gdi_resource vertices;
  gdi_resource cbuf;
  gdi_memory buf;
  size_t n_indices;
};

struct gk_texture_batch{
  std::vector<gdi_resource> handles;
  gdi_memory memory;
  size_t cursor;
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

scene scene{};

gdi_device driver{};
gdi_encoder encoder{};

std::vector<gk_draw_data> drawlist;

gdi_transform t1;
glm::mat4 m1;

gk_texture_batch textures;

struct gk_constant_buffer{
  uint32_t transform_offset;
};

gdi_rpool res_pool;

gk_draw_data construct_drawable(const Prefab& fab){
  gk_draw_data ret{};

  ret.texture = textures.handles[0];
  ret.n_indices = fab.n_indices;
  size_t geometry_bytes = fab.vertice_bytes + fab.indice_bytes;
  size_t cbuf_bytes = sizeof(gk_constant_buffer);

  // Create memory
  gdi_allocation_desc d_memory;
  d_memory.n_bytes = 2 * MiB;
  d_memory.usage = gdi_memory_usage_type::linear;
  d_memory.type = gdi_memory_type::host_write;
  gdi_memory memory = driver.allocate_memory(d_memory);
  ret.buf = memory;
  //----------------------

  gdi_buffer_desc d_resource{};
  size_t cursor = 0;

  // vertex
  d_resource.n_bytes = fab.vertice_bytes;
  ret.vertices = driver.create_buffer(memory, d_resource, cursor);
  cursor += fab.vertice_bytes;

  // index
  d_resource.n_bytes = fab.indice_bytes;
  ret.indices = driver.create_buffer(memory, d_resource, cursor);
  cursor += fab.indice_bytes;

  //constant buffer
  auto n = gk_constant_buffer{0};
  cursor = bk::align_p2(cursor, 256);
  d_resource.n_bytes = sizeof(gk_constant_buffer);
  ret.cbuf = driver.create_buffer(memory, d_resource, cursor);
  cursor += sizeof(gk_constant_buffer);

  void* vbo = driver.map_resource(ret.vertices, gdi_resource_type::buffer);
  void* ibo = driver.map_resource(ret.indices, gdi_resource_type::buffer);
  void* cbuf = driver.map_resource(ret.cbuf, gdi_resource_type::buffer);

  memcpy(ibo, fab.indices, fab.indice_bytes);
  memcpy(vbo, fab.vertices, fab.vertice_bytes);
  memcpy(cbuf, &n, sizeof(gk_constant_buffer));
  
  return ret;
}

void create_texture_batch(){
  gdi_allocation_desc d_alloc{};
  d_alloc.n_bytes = 100 * MiB;
  d_alloc.type = gdi_memory_type::host_write;
  d_alloc.usage = gdi_memory_usage_type::texel;
  textures.memory = driver.allocate_memory(d_alloc);

  textures.cursor = 0;

  fs_image_data image = fs_load_image("data/builtin_materials/default/image.png");
  if(!image.data){ _juye_crashf("failed to load image");}

  gdi_texture_desc d_texture{};
  d_texture.width = image.width;
  d_texture.height = image.height;
  d_texture.depth = 1;
  d_texture.mips = 1;

  gdi_resource tex = driver.create_texture(textures.memory, d_texture, textures.cursor);
  textures.cursor += image.bytes;

  driver.write_texture(image.data, tex, image.width, image.height, 0);
  textures.handles.push_back(tex);
  fs_unload_image(image);
}

void render_fe_begin(){
  driver.init_driver();
  create_texture_batch();

  res_pool = driver.allocate_resource_pool();
  encoder = driver.create_encoder();
  //
  drawlist.push_back(construct_drawable(scene.cube_default));
  m1 = glm::mat4(1);
  t1 = driver.generate_transform();
}


void sudo(){
  // auto tex = driver.get_display_texture();
  //
  encoder.reset();// we store the actual memory in a sep structure so no need to sync these.
  encoder.record();
  //
  // Renderpass structure
  // encoder.set_renderpass();
  
  // Arugment structure
  // encoder.set_resource_table();
  
  // // state(cull mode, pipeline, winding etc)
  // encoder.set_pso();
   gdi_viewport vp{1920, 1080, 0, 0, 0, 1.0};
   encoder.set_viewport(vp);
  
  // Draw
  
  // // submission, frame advance, syc, etc...
  // driver.submit(encoder);
  //
}

void render_fe_tick(){


  float* p = glm::value_ptr(scene.camera.projection);
  float* v = glm::value_ptr(scene.camera.view);
  driver.set_projection(v, p);
  glm::mat4 a = glm::rotate(m1, 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));
  m1 = a;
  driver.write_transform(t1, glm::value_ptr(m1));
  gk_draw_data d = drawlist[0];
  driver.dummy_draw(d.vertices, d.indices, d.n_indices, t1, d.cbuf, d.texture, res_pool);
  driver.submit_frame();
}

void render_fe_end(){

}

}
