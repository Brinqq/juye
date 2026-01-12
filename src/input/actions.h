#pragma once
#include "core/drivers/keyboard.h"

#include "core/drivers/display.h"
#include "bcl/containers/bucket.h"

#define JUYE_MAX_KEYS_PER_ACTION 2

namespace juye{
typedef void* inpAction;

class inpActionSystem{
  struct ActionEntry{
    juye::KeyCode codes[JUYE_MAX_KEYS_PER_ACTION];
  };

  DisplayDriver* mDisplay;
  bk::bucket<ActionEntry, 20> mActions;
public:
  int Init(DisplayDriver* pDisplay);
  inpAction Add(juye::KeyCode code);
  void Remove(inpAction action);
  bool Check(inpAction action);
  bool IsValid(inpAction action);

};

  // typedef uint16_t inpActionHandle;
  //
  // inpActionHandle CreateAction(); 
  // void DestroyAction(inpActionHandle);
  // int MapAction(inpActionHandle handle, KeyCode code);
  // void UnmapAction(inpActionHandle, KeyCode code);
  // int CheckAction(inpActionHandle handle);
}
