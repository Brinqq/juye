#include "immediate.h"

#include "juye/gk/prefabs.h"
#include "core/fsystem/file.h"
#include "core/configuration/build_generation.h"

#include <string>

#include "glm/gtc/matrix_transform.hpp"

//TODO: Add mapping for albedo enum to image lookup.

using namespace juye;

#include "bk/containers/cache.h"

static constexpr int kBuiltinTextureCount = 1;

static bk::in_cache<int, gkMesh, 5> gMeshCache;
static bk::in_cache<int, gkTexture, 5> gTextureCache;
static ImageData gImages[kBuiltinTextureCount];

static gkTexture SetTexture(const ImageData& image){
  gkTexture ret;
  ret.data = image.data;
  ret.height = image.height;
  ret.width = image.width;
  return ret;
}

void gkImmediateDrawList::SetDrawMode3D(){}

void gkImmediateDrawList::DrawCube(juye::vec3<float> pos, float scale, const gkImmediateAlbedo albedo){
  DrawEntry& entry = mImBuf->meshes[mImBuf->numMeshes];
  mImBuf->numMeshes++;

  Prefab cube = Prefab::Builder()
               .SetMesh(Prefab::PrefabMeshCube)
               .Build();

  glm::mat4 matrix(1);
  matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));
  matrix = glm::translate(matrix, glm::vec3(pos.x, pos.y, pos.z));
  memcpy(&entry, &matrix, sizeof(glm::mat4));

  entry.Model.mesh = cube.mesh;
  entry.Model.texture = SetTexture(gImages[static_cast<int>(albedo)]);
}

gkImmediateDrawList::gkImmediateDrawList(){
  mImBuf = new ImmediateBuffer();
  std::string texpath2(_SSF_GENERATED_TEXTURE_FOLDER);
  gImages[0]  = LoadI(texpath2.append("orange.png").c_str());
  if(gImages[0].data == nullptr){
    // _juye_runtime_error('h
  }
}

gkImmediateDrawList::~gkImmediateDrawList(){
  UnloadImage(gImages[0]);
  delete mImBuf;
}
