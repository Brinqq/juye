#pragma once

#define __JUYE_METAL 1
#include "imst.h"

#include <stdint.h>

// API Design Structure
// Retained
//  - GPU allocated resouces
//
// Immediate
//  - Draw list
//  - Draw Entries?

#if __JUYE_METAL

#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "drivers/metal/def.h"

namespace juye{


struct gdi_device{
  int init_driver();
  gdi_memory allocate_memory(void* fill, size_t n_bytes, const gdi_memory_flags flags);

  void write_memory(void* src, gdi_memory dst, size_t n_bytes, size_t offset);
  
  void set_viewport(float w, float h, float n, float f, float x, float y);
  void set_projection(const float* v, const float* p);

  void dummy_draw(void* vertices, void* indices, size_t n_indices);
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

};

}
#endif

#if __JUYE_VULKAN
#endif


