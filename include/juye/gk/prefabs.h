#pragma once

#include <stdint.h>
#include <stddef.h>
#include <gk/model/model.h>

namespace juye{

struct Prefab{
private:
public:
  float* pVertices;
  uint16_t* pIndices;
  uint32_t vertices;
  uint32_t stride;
  uint32_t indices;
  gkMesh mesh;

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
    float* pVertices;
    uint16_t* pIndices;
    uint32_t vertices;
    uint32_t stride;
    uint32_t indices;
  public:
    Builder(){};
    Builder(const Builder& rhs){};
    Builder(Builder&& rhs){};

    Builder& SetMesh(const BuiltinMeshType type) noexcept;
    Prefab Build() noexcept;
  };

};

}
