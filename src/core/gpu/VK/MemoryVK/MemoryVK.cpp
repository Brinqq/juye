#include "MemoryVK.h"

//for now we just grab a whole page each allocation.
VkResult MemoryVK::Allocate(VkDevice device, VkDeviceMemory& handle, VkDeviceSize bytes){
  VkMemoryAllocateInfo mem{};
  mem.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  mem.pNext = 0;
  mem.allocationSize = bytes;
  mem.memoryTypeIndex = 0;
  return(vkAllocateMemory(device, &mem, nullptr, &handle));
}
