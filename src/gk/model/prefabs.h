#pragma once

#include <stdint.h>
#include <stddef.h>
#include <gk/model/model.h>

namespace juye{

struct Prefab{
private:
public:
  float* vertices;
  uint16_t* indices;
  uint16_t n_vertices;
  uint16_t n_indices;
  uint16_t stride;

  ~Prefab(){}

  enum BuiltinMeshType{
    PrefabMeshCube,
    PrefabMeshPlane,
  };

  enum BuiltinTextureType{
    BuilintTextureSilver,
  };

  class Builder{
  private:
    float* vertices;
    uint16_t* indices; //TODO: support different widths.
    uint32_t n_vertices;
    uint32_t n_indices;
    uint32_t stride;
  public:
    Builder(){};
    Builder(const Builder& rhs) = delete;
    Builder(Builder&& rhs) = delete;

    Builder& set_mesh(const BuiltinMeshType type) noexcept;
    Prefab build() noexcept;
  };

};

}
