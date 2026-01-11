#pragma once

#include "vulkan/vulkan.h"

namespace MemoryVK{
  VkResult Allocate(VkDevice device, VkDeviceMemory* handle, VkDeviceSize bytes, uint32_t type);
  void Deallocate(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* allocator);
}

namespace vak{

enum Error{
  VAK_SUCCESS = 0x00,
  VAK_INVALID_GPU = 0x01,
  VAK_INVALID_DEVICE = 0x02,
  VAK_OUT_OF_MEMORY = 0x03,
  VAK_UNSUPPORTED_MEMORYTYPE = 0x04,
  VAK_UNKNOWN_HEAPTYPE = 0x05,
  VAK_UNKOWN_ERROR = 0x06
};

enum class HeapType{
  HeapSmall,
  HeapMeduim,
  HeapLarge,
  HeapMax
};

  Error Init(VkDevice device, VkPhysicalDevice gpu);
  Error GetAMemoryTypeIndex(VkMemoryPropertyFlags flags);
  void Destory();
  // int SwitchDevice(VkDevice device);
  // int SwitchGPU(VkDevice gpu);
}
