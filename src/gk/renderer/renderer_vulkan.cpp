#include "renderer.h"


using namespace juye;

int gkVulkanFrontend::Init(void* pDisplayHandle){
  mBackend.Init(pDisplayHandle);
  return 0;
}

void gkVulkanFrontend::Destroy(){
  mBackend.Destroy();
}

void gkVulkanFrontend::DrawImmediateBuffer(const gkImmediateDrawList& buf){
}


//FIXME lease delete later
//=======================================================================================================================
//=======================================================================================================================

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "juye/gk/prefabs.h"
#include "core/configuration/build_generation.h"
#include "core/fsystem/file.h"
#include <string>

struct Camera{
  glm::mat4 view;
  glm::mat4 projection;
};

struct Meshk{
  VK::GeoHandle handle;
  glm::mat4 transform;
  glm::mat4 push[3];
};


Camera cam{};
Meshk simpleCube;
Meshk plane;
ImageData skyboxData[6];
ResourceHandle skybox;

CubeMapWriteDescription CubeMapDataGenerate(){
  skyboxData[5]  = LoadI("C:/main/.dev/projects/engine/juye/data/textures/ny.png");
  skyboxData[4]  = LoadI("C:/main/.dev/projects/engine/juye/data/textures/py.png");
  skyboxData[3]  = LoadI("C:/main/.dev/projects/engine/juye/data/textures/pz.png");
  skyboxData[2]  = LoadI("C:/main/.dev/projects/engine/juye/data/textures/nz.png");
  skyboxData[1]  = LoadI("C:/main/.dev/projects/engine/juye/data/textures/nx.png");
  skyboxData[0]  = LoadI("C:/main/.dev/projects/engine/juye/data/textures/px.png");

  CubeMapWriteDescription ret{};
  for(int i = 0; i < 6; ++i){
    if(skyboxData[i].data == nullptr){juye_runtime_error();}
    ret.data[i] = skyboxData[i].data;
    ret.bytes[i] = skyboxData[i].bytes;
  }

  return ret;
}

void CubeMapDataCleanup(){
  for(int i = 0; i < 6; ++i){
    juye::UnloadImage(skyboxData[i]);
  }
}

void gkVulkanFrontend::Dev(){
  cam.view = glm::lookAt(glm::vec3(0.0f, -4.0f, -4.0f), glm::vec3(0.0f, 1.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  cam.projection = glm::perspectiveFov(glm::radians(60.0f), static_cast<float>(1424), static_cast<float>(714), 0.1f, 1000.0f);

  simpleCube.push[0] = glm::mat4(1);
  simpleCube.push[1] = cam.view;
  simpleCube.push[2] = cam.projection;

  plane.push[0] = glm::mat4(1);
  plane.push[1] = cam.view;
  plane.push[2] = cam.projection;


  simpleCube.push[0] = glm::translate(simpleCube.push[0], glm::vec3(0.0f, -1.0f, 2.0f));
  simpleCube.push[0] = glm::rotate(simpleCube.push[0], glm::radians(180.0f),glm::vec3(1.0f, 0.0f, 1.0f));
  // simpleCube.push[0] = glm::scale(simpleCube.push[0], glm::vec3(0.2f));

  plane.push[0] = glm::scale(plane.push[0], glm::vec3(20.0f, 0.0f, 20.0f));

  Prefab cube = Prefab::Builder()
               .SetMesh(Prefab::PrefabMeshCube)
               .Build();

  Prefab planePrefab = Prefab::Builder()
               .SetMesh(Prefab::PrefabMeshCube)
               .Build();

  std::string texpath(_SSF_GENERATED_TEXTURE_FOLDER);
  std::string texpath2(_SSF_GENERATED_TEXTURE_FOLDER);
  ImageData image  = LoadI(texpath.append("def.png").c_str());
  ImageData pimage  = LoadI(texpath2.append("orange.png").c_str());

  GeometryData dat;
  GeometryData geometryPlane;

  dat.pVertex = (void*)cube.pVertices;
  dat.pIndices = (void*)cube.pIndices;
  dat.indicesBytes = cube.indices * sizeof(uint16_t);
  dat.vertexBytes = cube.vertices * cube.stride;
  dat.numIndices = cube.indices;
  
  dat.texture = image.data;
  dat.textureWidth = image.width;
  dat.textureHeight = image.height;

  geometryPlane.pVertex = (void*)planePrefab.pVertices;
  geometryPlane.pIndices = (void*)planePrefab.pIndices;
  geometryPlane.indicesBytes = planePrefab.indices * sizeof(uint16_t);
  geometryPlane.vertexBytes = planePrefab.vertices * planePrefab.stride;
  geometryPlane.numIndices = planePrefab.indices;
  geometryPlane.texture = pimage.data;
  geometryPlane.textureWidth = pimage.width;
  geometryPlane.textureHeight = pimage.height;

  simpleCube.handle = mBackend.CreateGeometry(dat);
  plane.handle = mBackend.CreateGeometry(geometryPlane);

  mBackend.MapGeometryPassPushBuf(simpleCube.handle, simpleCube.push);
  mBackend.MapGeometryPassPushBuf(plane.handle, plane.push);

  UnloadImage(image);
  UnloadImage(pimage);

  CubeMapWriteDescription x = CubeMapDataGenerate();
  skybox =  mBackend.CreateCubeMap(skyboxData[0].width);
  mBackend.WriteCubeMap(skybox ,x);
  mBackend.SetSkyBox(skybox);
  CubeMapDataCleanup();
}

void gkVulkanFrontend::DevUpdate(){
   mBackend.WriteFrustum(reinterpret_cast<float*>(&cam));
   mBackend.AddToDrawList(simpleCube.handle);
   mBackend.AddToDrawList(plane.handle);
   mBackend.Draw();
}

//=======================================================================================================================
//=======================================================================================================================
