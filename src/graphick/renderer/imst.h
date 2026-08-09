#pragma once
#include <stddef.h>


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
//

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
// void write_texture()
// void write_buffer()
// void zero_buffer()
// void zero_texture()
// void readback_buffer()
// void readback_texture()
// void copy_buffer()
// void copy_texture()
// void bind_pso()
// void drawlist_begin()
// void draw()
// void draw_indirect()
// void drawlist_end()
// void submit()


typedef void* gdi_memory;
typedef size_t gdi_transform;
typedef void* gdi_rpool;

enum gdi_memory_flags{
};

struct gdi_projection{
  float view[16];
  float proj[16];
};


struct DrawDescriptor{};
