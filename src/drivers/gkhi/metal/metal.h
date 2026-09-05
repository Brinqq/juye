#pragma once
#include "drivers/gkhi/types.h"

#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

namespace juye{

struct gdi_device;

struct gdi_encoder{
public:
 void set_viewport(const gdi_viewport& vp);
 void reset();
 void record();
 void commit();

private:
  MTL4::CommandBuffer* storage;
  MTL4::RenderCommandEncoder* commands;
};

struct gdi_device{

  int init_driver();

  gdi_memory allocate_memory(const gdi_allocation_desc& memory);
  void deallocate_memory(gdi_memory mem);

  gdi_resource create_buffer(gdi_memory mem, const gdi_buffer_desc& d_buf, size_t offset);
  void destory_buffer(gdi_resource buffer);

  gdi_resource create_texture(gdi_memory mem, const gdi_texture_desc& d_tex, size_t offset);
  void destory_texture();

  gdi_resource create_attachment(gdi_memory mem, const gdi_attachment_desc& d_tex, size_t offset);
  void destory_attachment();

  void* map_resource(gdi_resource res, gdi_resource_type type);
  void unmap_resource(gdi_resource res, gdi_resource_type type);

  gdi_texture get_display_texture();

  gdi_renderpass create_renderpass();
  gdi_pipeline create_pipeline();
  gdi_encoder create_encoder();

  //------------------------------------------------------------------------------------
  gdi_memory allocate_buffer(gdi_rpool pool, size_t n_bytes, const uint64_t flags);
  gdi_memory allocate_texture(gdi_rpool pool, size_t w, size_t h, uint32_t mips);
  gdi_rpool  allocate_resource_pool();
  gdi_transform generate_transform();
  void write_texture(void* src, gdi_memory mem, size_t width, size_t height, uint32_t mip);
  void write_transform(gdi_transform t, void* src);
  void commit_resource_pool(gdi_rpool pool);
  void set_constant_buffer(gdi_memory buf);
  void set_viewport(float w, float h, float n, float f, float x, float y);
  void set_projection(const float* v, const float* p);

  void dummy_draw(void* vertices, void* indices, size_t n_indices, gdi_transform transform, gdi_memory user_cbuf,
  gdi_memory texture, gdi_rpool u_pool);
  void submit_frame();
  void execute_single_draw();
  //------------------------------------------------------------------------------------
  
private:
  friend struct gdi_encoder;
  void generate_renderpasses();
  void next_frame();

  MTL::Device* device = nullptr;

  CA::MetalLayer* display_layer;
  void* window;
  MTL::Texture* display_texture;

  MTL::ResidencySet* set;
  MTL::Viewport viewport;

  MTL::Buffer* llcb;
  MTL::Buffer* transform_buf;
  size_t transform_offset = 0; //temp

};

}
