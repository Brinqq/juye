#pragma once

// Texture
// Draw Data Buffer(index / vertex)
// User Defined

typedef void* gdi_memory;

enum gdi_memory_flags{
  GDI_MEMORY_UNDEFINED_BIT  = 0x0,
  GDI_MEMORY_LINEAR_BIT = 0x1
};

struct gdi_projection{
  float view[16];
  float proj[16];
};
