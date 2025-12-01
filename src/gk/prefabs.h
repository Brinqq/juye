#pragma once

#include <stdint.h>
#include <stddef.h>

namespace ssf{
  namespace prefabs{

template<typename _IndexType>

struct TexturedPlane {
    static constexpr float vertices[] = {
        -0.5f, 0.0f, -0.5f,   0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, 0.0f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, 0.0f,  0.5f,   1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };

    static constexpr _IndexType indices[6] = {
        0, 1, 2,   2, 3, 0
    };

    static constexpr uint32_t nVertices = 4;
    static constexpr uint32_t Vertexstride = sizeof(float) * 8;
    static constexpr size_t VertexBytes = Vertexstride * nVertices;

    static constexpr uint32_t nIndices = 6;
    static constexpr uint32_t IndiceStride = sizeof(_IndexType);
    static constexpr size_t IndiceBytes = IndiceStride * nIndices;
};


template<typename _IndexType>
struct TexturedCube{
  static constexpr float vertices[] = {  // 24 vertices × 5 floats
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

  static constexpr _IndexType indices[36] = {
      0,  1,  2,   2,  3,  0,   // Front
      4,  5,  6,   6,  7,  4,   // Back
      8,  9, 10,  10, 11,  8,   // Top
     12, 13, 14,  14, 15, 12,   // Bottom
     16, 17, 18,  18, 19, 16,   // Right
     20, 21, 22,  22, 23, 20    // Left
  };

  static constexpr uint32_t nVertices = 24;
  static constexpr uint32_t Vertexstride = sizeof(float) * 8;
  static constexpr size_t VertexBytes = Vertexstride * nVertices;

  static constexpr uint32_t nIndices = 36;
  static constexpr uint32_t IndiceStride = sizeof(_IndexType);
  static constexpr size_t IndiceBytes = IndiceStride * nIndices;
};


  template<typename _IndexType>
  struct StandardCube{
    static constexpr float vertices[24] = {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    static constexpr _IndexType indices[36] = {
        0, 1, 2,  2, 3, 0,
        5, 4, 7,  7, 6, 5,
        3, 2, 6,  6, 7, 3,
        4, 5, 1,  1, 0, 4,
        1, 5, 6,  6, 2, 1,
        4, 0, 3,  3, 7, 4
    };

    static constexpr uint32_t nVertices = 8;
    static constexpr uint32_t Vertexstride = sizeof(float) * 3;
    static constexpr size_t VertexBytes = Vertexstride * nVertices;

    static constexpr uint32_t nIndices = 36;
    static constexpr uint32_t IndiceStride = sizeof(_IndexType);
    static constexpr size_t IndiceBytes = IndiceStride * nIndices;
};



  };
};
