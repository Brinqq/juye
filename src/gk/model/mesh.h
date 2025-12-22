#pragma once

#include "juye/gk/primitives.h"
#include "core/global.h"


namespace juye{

struct Mesh{
  void* mVertices;
  uint32_t mNumVerties;
  void* mIndices;
  uint32_t mNumIndices;
};

};
