#include "MemoryVK.h"
#include "internal.h"
#include "assert.h"

//for now we just grab a whole page each allocation.
VkResult MemoryVK::Allocate(VkDevice device, VkDeviceMemory* handle, VkDeviceSize bytes, uint32_t type){
  const VkMemoryAllocateInfo mem{
   .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
   .pNext = 0,
   .allocationSize = bytes,
   .memoryTypeIndex = type
  };

  return vkAllocateMemory(device, &mem, nullptr, handle);
}

void MemoryVK::Deallocate(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* allocator){
  vkFreeMemory(device, memory, allocator);
}


using namespace vak;

namespace vak{
  vakDeviceContext gDeviceContext;
}

Error vak::Init(VkDevice device, VkPhysicalDevice gpu){
  if(device == VK_NULL_HANDLE)return VAK_INVALID_DEVICE;
  if(gpu == VK_NULL_HANDLE)return VAK_INVALID_GPU;

  VkPhysicalDeviceProperties gpuProperties;
  VkPhysicalDeviceMemoryProperties memoryProperties;

  vkGetPhysicalDeviceProperties(gpu, &gpuProperties);
  vkGetPhysicalDeviceMemoryProperties(gpu, &memoryProperties);

  gDeviceContext.device = device;
  gDeviceContext.gpu = gpu;
  gDeviceContext.maxAllocations = gpuProperties.limits.maxMemoryAllocationCount;
  gDeviceContext.pageGranularity = gpuProperties.limits.bufferImageGranularity;
  gDeviceContext.heapCount = memoryProperties.memoryHeapCount;


  for(int i = 0; i < gDeviceContext.heapCount; i++){
    VkMemoryHeap heap = memoryProperties.memoryHeaps[i];
    gDeviceContext.write.heaps[i].availableBytes = heap.size;
    gDeviceContext.write.heaps[i].isLocal = heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ? true : false;
  }

  for(int i = 0; i < memoryProperties.memoryTypeCount; ++i){
    uint32_t key = memoryProperties.memoryTypes[i].propertyFlags;
    uint8_t idx = memoryProperties.memoryTypes[i].heapIndex;
    uint8_t type = i;
    if(key == 0) continue;

    auto x = gDeviceContext.memoryTypeToHeapMap.find(key);

    // not found
    if(x == gDeviceContext.memoryTypeToHeapMap.end()){
      vakHeapMapEntry entry{};
      entry.indices[entry.count] = idx;
      entry.count++;
      gDeviceContext.memoryTypeToHeapMap.insert(std::pair{key, entry});
    }else{
      assert(x->second.count < kMaxHeapEntries);
      x->second.indices[x->second.count] = idx;
      x->second.count++;
    }

    auto types = gDeviceContext.flagToTypeMap.find(key);
    if(types == gDeviceContext.flagToTypeMap.end()){
      vakMemoryPropMapEntry entry{};
      entry.indices[entry.count] = type;
      entry.count++;
      gDeviceContext.flagToTypeMap.insert(std::pair(key, entry));
    }else{
      assert(types->second.count < kMaxTypeEntries);
      types->second.indices[types->second.count] = type;
      types->second.count++;
    }

  }

  return VAK_SUCCESS;
};

int GetAMemoryTypeIndex(VkMemoryPropertyFlags flags){
  return vak::GetAvailableMemoryType(0, flags);
}

void Destory(){

}

