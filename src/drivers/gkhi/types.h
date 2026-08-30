#pragma once
#include "base/global.h"

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
