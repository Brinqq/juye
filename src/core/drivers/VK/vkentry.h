#pragma once

#include "juye/gk/primitives.h"

#include <list>

namespace juye::driver{

enum LightEntryType{
  LightEntryAmbient,
  LightEntryDirectional,
};


struct LightEntryWriteDescription{
  
};

struct GeometryWriteDescription{

};

struct LightEntry{
  Color3 color;
  Vector3f position;
};

struct GeometryEntry{

};


struct LightEntryListBundle{
  std::list<LightEntry> ambient;
  std::list<LightEntry> directional;
};


}
