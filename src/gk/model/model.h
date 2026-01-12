#pragma once
#include <stdint.h>

struct gkMesh{
  float* pVertices;
  uint16_t* pIndices;
  uint16_t numVertices;
  uint16_t numIndices;
  uint16_t stride;
};

struct gkTexture{
  void* data;
  uint64_t width;
  uint64_t height;
};

struct gkModel{
  gkMesh mesh;
  gkTexture texture;
};
