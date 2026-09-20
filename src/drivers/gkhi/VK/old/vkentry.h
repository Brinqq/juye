#pragma once

#include "core/primitives.h"

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
  juye::vec3<float> color;
  juye::vec3<float> position;
};

struct GeometryEntry{

};


struct LightEntryListBundle{
  std::list<LightEntry> ambient;
  std::list<LightEntry> directional;
};


}
