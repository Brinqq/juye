#pragma once
#include "gk/model/model.h"

namespace juye{

struct gkCamera{};

typedef void* gkSceneNode;

class gkScene{
private:

  gkCamera* mCameras;
  float* mVertices;
  uint16_t* mIndices;
  void* mTexels;

  uint32_t mCameraCount;
  uint32_t mModelCount;
public:
 gkSceneNode AddCamera();
};

}
