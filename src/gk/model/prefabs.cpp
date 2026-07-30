#include "prefabs.h"

using namespace juye;

static float k_plane_vertices[32] = {
  -0.5f, 0.0f, -0.5f,   0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
   0.5f, 0.0f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
   0.5f, 0.0f,  0.5f,   1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};

static uint16_t k_plane_indices[6] = {
  0, 1, 2, 2, 3, 0
};


static float k_cube_vertices[192] = {  // 24 vertices × 5 floats
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  
   0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
   0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
  
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
  
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  
   0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
};


static  uint16_t k_cube_indices[36] = {
    0,  1,  2,   2,  3,  0,   
    4,  5,  6,   6,  7,  4,   
    8,  9, 10,  10, 11,  8,   
   12, 13, 14,  14, 15, 12,   
   16, 17, 18,  18, 19, 16,   
   20, 21, 22,  22, 23, 20    
};


Prefab Prefab::Builder::build() noexcept{
  Prefab ret;
  ret.vertices = vertices;
  ret.indices = indices;
  ret.n_indices = n_indices;
  ret.n_vertices = n_vertices;
  ret.stride = stride;
  return ret;
};

Prefab::Builder& Prefab::Builder::set_mesh(const Prefab::BuiltinMeshType type) noexcept{
  switch (type){
    case PrefabMeshCube:
    this->indices = k_cube_indices;
    this->vertices = k_cube_vertices;
    this->n_indices = 36;
    this->n_vertices = 24;
    this->stride = sizeof(float) * 8;
    return *this;

    case PrefabMeshPlane:
    this->vertices = k_plane_vertices;
    this->indices = k_plane_indices;
    this->n_indices = 6;
    this->n_vertices = 4;
    this->stride = sizeof(float) * 8;
    return *this;
    }

  
  return *this;
}
