#pragma once
#include "base/global.h"


struct gdi_projection{
  float view[16];
  float proj[16];
};

enum class gdi_memory_type{
  local,
  host_write,
  host_readback,
};

enum class gdi_memory_usage_type{
  texel,
  linear,
  attachment,
};


enum  gdi_memory_flags{
  GDI_MEMORY_FLAGS_NONE = 0
};;

enum class gdi_format_type{
  
};

enum gdi_resource_type{
  texture,
  buffer,
  attachment,
};

enum class gdi_topology_type{
  wireframe,
  trianglelist,
};

enum gdi_winding_order_type{
  clockwise,
  counter_clockwise,
};

struct gdi_shader{};

struct gdi_pipeline_state_desc{
  gdi_shader* vertex_shader;
  gdi_shader* pixel_shader;
  gdi_shader* mesh_shader;
  gdi_winding_order_type winding;
  gdi_topology_type topology;
};

struct gdi_vertex_attribute_desc{};

struct gdi_allocation_desc{
  size_t n_bytes;
  gdi_memory_type type;
  gdi_memory_usage_type usage;
};


struct gdi_resource_desc{
  gdi_resource_type type;
  size_t n_bytes;
};


struct gdi_buffer_desc{
  size_t n_bytes;
};



struct gdi_texture_desc{
  size_t width;
  size_t height;
  int depth;
  int array = 1;
  int mips;
};

struct gdi_texture{
  size_t width();
  size_t height();
  gdi_texture_desc descriptor();

private:
  void* handle;
  void* resource;
};

// texture* create_texture();

struct gdi_attachment_desc{

};

struct gdi_renderpass{

};

struct gdi_viewport{
  float width;
  float height;
  float xs;
  float ys;
  float near;
  float far;
};


struct gdi_pipeline{
private:


};
typedef uint32_t gdi_flags;
typedef void* gdi_memory;
typedef void* gdi_resource;
typedef size_t gdi_transform;
typedef void* gdi_rpool;
