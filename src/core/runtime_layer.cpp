#include "runtime_layer.h"

#include "drivers/display.h"
#include "core/drivers/VK/vk_core.h"

bool kRuntimeExit = false;

typedef void* DisplayHandle;

int juye::StartRuntime(){
  juye::DisplayDriver* pDisplay = new DisplayWin32();
  pDisplay->Init();
  DisplayHandle handle = pDisplay->Handle();

  VK v{};
  v.Init(handle);

  // while(!kRuntimeExit && pDisplay->IsRunning()){
  //   pDisplay->Update();
  // };

  v.Destroy();
  pDisplay->Destroy();
  printf("Program Exited successful.\n");
  return 0;
}
