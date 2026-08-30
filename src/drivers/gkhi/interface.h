#pragma once
#include "types.h"


//===========================
// Types
//===========================
// opaque handle =  qh
// struct = struct
// resources = res

//===========================
// Resources
//===========================
// qh allocate_buffer()
// qh allocate_texture2D()
// qh allocate_pool()
// qh allocate_resource_from_pool()

// void write_texture()
// void write_buffer()
// void zero_buffer()
// void zero_texture()
// void readback_buffer()
// void readback_texture()
// void copy_buffer()
// void copy_texture()

// void deallocate_buffer()
// void deallocate_texture2D()
// void deallocate_pool()
// void deallocate_resource_from_pool()

//===========================
// Sync
//===========================
//
//

//===========================
// Pipeline state
//===========================
//
//

//===========================
// Presentation
//===========================
// struct create_viewport()
// res get_draw_texture()
// void resize_draw_texture()
// void swap_draw_texture()

//===========================
// command device
//===========================
// void set_pso()
// void set_argument_table()

// void drawlist_begin()
// void draw()
// void draw_indirect()
// void drawlist_end()
// void submit()

// API Design Structure
// Retained
//  - GPU allocated resouces
//
// Immediate
//  - Draw list
//  - Draw Entries?

#define __JUYE_METAL 1
#if __JUYE_METAL
#define METAL_DEBUG_LAYER 1

#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

namespace juye{

struct gdi_device{
  int init_driver();

  gdi_memory allocate_buffer(gdi_rpool pool, size_t n_bytes, const uint64_t flags);
  gdi_memory allocate_texture(gdi_rpool pool, size_t w, size_t h, uint32_t mips);
  gdi_rpool  allocate_resource_pool();
  gdi_transform generate_transform();

  void write_buffer(void* src, gdi_memory dst, size_t n_bytes, size_t offset);
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
  
private:
  void generate_renderpasses();
  void next_frame();

  MTL::Device* device = nullptr;
  CA::MetalLayer* display_layer;
  MTL::Viewport viewport;
  void* window;

  MTL::Buffer* llcb;
  MTL::Buffer* transform_buf;
  size_t transform_offset = 0; //temp

};

}
#endif

#if __JUYE_VULKAN
#endif


