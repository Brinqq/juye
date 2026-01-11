#include "core/drivers/VK/MemoryVK/MemoryVK.h"
#include "internal.h"

using namespace vak;
namespace vak{extern vakDeviceContext gDeviceContext;}

int vak::GetAvailableMemoryType(uint64_t bytes, VkMemoryPropertyFlags bits){
  auto type = gDeviceContext.flagToTypeMap.find(bits);
  if(type == gDeviceContext.flagToTypeMap.end()){
    return -1;
  }

  for(int i = 0; i < type->second.count; i++){
    auto heaps = gDeviceContext.memoryTypeToHeapMap.find(type->second.indices[i]);
    if(heaps == gDeviceContext.memoryTypeToHeapMap.end()){return -1;}
    for(int k = 0; k < type->second.count; k++){
      //TODO: checks sizes
      return type->second.indices[i];
    };


  }
  

  return -1;
}

uint64_t vak::GetHeapSize(const HeapType type){
  switch (type){
  case HeapType::HeapSmall:
    return gDeviceContext.config.smallHeapSize;
  case HeapType::HeapMeduim:
    return gDeviceContext.config.meduimHeapSize;
  case HeapType::HeapLarge:
    return gDeviceContext.config.largeHeapSize;
  case HeapType::HeapMax:
    return gDeviceContext.config.maxHeapSize;
  defualt:
    return 0;
  }
}
