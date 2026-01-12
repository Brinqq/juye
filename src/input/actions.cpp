#include "actions.h"

#include "core/drivers/display.h"

using namespace juye;

int inpActionSystem::Init(DisplayDriver* pDisplay){
  mDisplay = pDisplay;
  return 0;
}

inpAction inpActionSystem::Add(KeyCode code){
  ActionEntry* entry = &mActions.construct();
  entry->codes[1] = code;
  return entry;
}

void inpActionSystem::Remove(inpAction action){
  mActions.destruct(static_cast<ActionEntry*>(action));
  action = nullptr;
}

bool inpActionSystem::Check(inpAction action){
    return mDisplay->PollKey(static_cast<KeyCode>(_juye_translate_key(static_cast<ActionEntry*>(action)->codes[0])));
}

bool inpActionSystem::IsValid(inpAction action){
  return !(action == nullptr);
}
