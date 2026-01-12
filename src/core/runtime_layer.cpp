#include "runtime_layer.h"
#include "gk/renderer/renderer.h"
#include "gk/immediate/immediate.h"
#include "gk/scene/scene.h"
#include "input/actions.h"

#include "drivers/display.h"
#include "core/drivers/VK/vk_core.h"

bool kRuntimeExit = false;
using namespace juye;

struct Engine{
 DisplayDriver* pDisplay;
 gkRenderer* pRenderer;
 inpActionSystem actions;
};

 Engine engine;
 gkImmediateDrawList imm;


typedef void* DisplayHandle;

void AddToDrawlist(){
  imm.SetDrawMode3D();
  imm.DrawCube(vec3{0.0f, 0.0f, 0.0f}, 20, gkImmediateAlbedo::Orange);
}

static void StartSys(){
  engine.pDisplay = new DisplayWin32();
  engine.pDisplay->Init();
  DisplayHandle handle = engine.pDisplay->Handle();
  engine.pRenderer = new gkVulkanFrontend();
  engine.pRenderer->Init(handle);
  engine.actions.Init(engine.pDisplay);
}

static void Update(){
  while(!kRuntimeExit && engine.pDisplay->IsRunning()){
    AddToDrawlist();
    engine.pDisplay->Update();
  };
}

static void Cleanup(){
  engine.pRenderer->Destroy();
  engine.pDisplay->Destroy();
  delete engine.pRenderer;
  delete engine.pDisplay;
}

void SetUpInput(){

}

int juye::StartRuntime(){
  StartSys();
  Update();
  Cleanup();
  printf("Program Exited successful.\n");
  return 0;
}
