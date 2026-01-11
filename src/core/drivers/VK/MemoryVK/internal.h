#pragma once
#include "MemoryVK.h"

#include <stdint.h>
#include <vulkan/vulkan.h>
#include <bcl/containers/cache.h>
#include <unordered_map>
#include <atomic>
#include <bcl/memory/bkmemory.h>

#define VAK_DEFAULT_SMALL_HEAP_SIZE (KiB * 64)
#define VAK_DEFAULT_MEDIUM_HEAP_SIZE (MiB * 1)
#define VAK_DEFAULT_LARGE_HEAP_SIZE (MiB * 128)
#define VAK_DEFAULT_MAX_HEAP_SIZE (GiB * 2)

constexpr int kMaxHeapEntries = 7;
constexpr int kMaxTypeEntries = 4;

namespace vak{

struct vakHeap{
  uint64_t availableBytes;
  bool isLocal;
};

struct vakHeapMapEntry{ 
  uint8_t count; 
  uint8_t indices[kMaxHeapEntries];
};

struct vakMemoryPropMapEntry{ 
  uint8_t count; 
  uint8_t indices[kMaxTypeEntries]; //we assert this at runtime but, I have never seen a gpu go above this so lets just pray.
};

struct vakDeviceContext{
public:
  struct Configurable{
    uint32_t smallHeapSize = VAK_DEFAULT_SMALL_HEAP_SIZE;
    uint32_t meduimHeapSize = VAK_DEFAULT_MEDIUM_HEAP_SIZE;
    uint64_t largeHeapSize = VAK_DEFAULT_LARGE_HEAP_SIZE;
    uint64_t maxHeapSize = VAK_DEFAULT_MAX_HEAP_SIZE;
  };

  struct Writeable{
    std::atomic<uint32_t> allocationsCount;
    vakHeap heaps[VK_MAX_MEMORY_HEAPS]; 
  };

  VkDevice device;
  VkPhysicalDevice gpu;
  uint32_t maxAllocations;
  uint8_t heapCount;
  VkDeviceSize pageGranularity;

  //TODO: use a better hashmap.
  //this cannot rehash or grow after ::Init() else every allocators heap references gets invalidated.
  std::unordered_map<uint16_t, vakHeapMapEntry> memoryTypeToHeapMap;
  std::unordered_map<uint32_t, vakMemoryPropMapEntry> flagToTypeMap;

  //we pad out too a cacheline boundary here because everything past this point is writeable from multiple threads,
  //where as previous data is readonly after ::Init(). We dont want Writeable data and ReadOnly Data sharing cachelines
  //due to costly core synchronization.

  alignas(64) Configurable config;
  alignas(64) Writeable write;
};

int GetAvailableMemoryType(uint64_t bytes, VkMemoryPropertyFlags bits);
uint64_t GetHeapSize(const HeapType type);

}
