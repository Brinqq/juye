#pragma once

#include "core/primitives.h"
#include <stdint.h>

#define JUYE_GK_MAX_IMMEDIATE_DRAW_ENTRIES 20

class gkVertexBuffer{
};

enum class gkImmediateAlbedo{
  Orange
};

class gkImmediateDrawList{
  struct DrawEntry{
    gkModel Model;
    float transform[16];
  };

  struct ImmediateBuffer{
    DrawEntry meshes[JUYE_GK_MAX_IMMEDIATE_DRAW_ENTRIES];
    uint8_t numMeshes;
  };

public:
  juye::gkCamera mCamera;
  juye::gkScene mScene;
  ImmediateBuffer* mImBuf;
  gkTexture mTextures;

  void begin();
  void end();

public:
  // void SetDrawMode2D()
  // void SetCamera2DPos(juye::vec3<float> pos);
  // void SetCamera2DLookAt();
  // void SetSky2D();
  // void DrawCircle();
  // void DrawTriangle();
  // void DrawSquare();
  
  void SetDrawMode3D();
  void DrawCube(juye::vec3<float> pos, float scale, const gkImmediateAlbedo albedo);
  // void SetSky3D();
  // void SetCamera3DPos(juye::vec3<float> pos);
  // void SetCamera3DLookAt();
  // void DrawSphere();
  // void DrawPlane();
  // void DrawVertexBuffer();
  

  gkImmediateDrawList();
  ~gkImmediateDrawList();
};
