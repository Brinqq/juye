#include "actions.h"

#include "core/drivers/device.h"

#include <unordered_map>

#define _SSF_MAX_KEYS_PER_ACTION 2

struct Action{
  juye::KeyCode codes[_SSF_MAX_KEYS_PER_ACTION]{juye::KeyCodeNone, juye::KeyCodeNone};
  std::atomic<int> status;
};

//TODO: change to custom flat_map impl.
std::unordered_map<juye::ActionHandle, Action> ActionLUT;
juye::ActionHandle counter = 0;

namespace juye{

  ActionHandle CreateAction(){
    ActionHandle ret =  counter;
    ActionLUT[ret];
    counter++;
    return ret;
  }

  void DestroyAction(ActionHandle action){
    ActionLUT.erase(action);
  }

  int MapAction(ActionHandle handle, KeyCode code){
    Action& action = ActionLUT.at(handle);
    for(int i = 0; i < _SSF_MAX_KEYS_PER_ACTION; ++i){
      if(action.codes[i] == KeyCodeNone){
        action.codes[i] = code;
        PhysicalKeyMap.insert({_ssf_translate_key(code), &action.status});
        return 0;
      }
    }
    return 1;
  }
  
  void UnmapAction(ActionHandle handle, KeyCode code){
    Action& action = ActionLUT.at(handle);
    for(int i = 0; i < _SSF_MAX_KEYS_PER_ACTION; ++i){
      if(action.codes[i] == code){ 
        action.codes[i] = KeyCodeNone;
        PhysicalKeyMap.erase(code);
      }
    }
  }

  int CheckAction(ActionHandle handle){
    Action& action = ActionLUT.at(handle);
    return action.status;
  }
}


