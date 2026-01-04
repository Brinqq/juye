#pragma once

#include "vulkan/vulkan.h"
#include "vk_debug.h"

#include "bcl/containers/span.h"
#include "bcl/containers/vector.h"
#include "bcl/containers/bitset.h"
#include "bcl/containers/string.h"
#include <vector>

enum QueueBitTypes{
  QueueBitNone = 0x0,
  QueueBitGraphic = 0x2,
  QueueBitTransfer = 0x4,
  QueueBitCompute = 0x8,
  QueueBitPresent = 0x16,
  QueueBitSparse = 0x32
};


enum ShaderStageType{
  ShaderStageNone = - 1,
  ShaderStageVertex,
  ShaderStagePixel,
  ShaderStageCompute,
};

namespace juye::driver{

  enum VulkanInstanceLayerType{
    VulkanInstanceLayerValidation = 0x0
  };


  enum BuiltinUniformType{
    BuiltinUniformCamera,
    BuiltinUniformCount
  };

  struct DrawFrustum{
    float view[4]; //should this be in here? probably not, who knows.
    float projection[4];
  };

}

namespace juye{

//TODO:Add AMD 256MiB memory extenstion support
enum class vlkHeapMemoryType{
  HeapMemoryVRAM,
  HeapCached,
  HeapHostUnCached,
  HeapHostMapped,
};

enum vlkGPUBitFlags{
  GpuDiscreteBit
};

struct vlkHeapStructure{
  struct Heap{
    uint64_t bytes;
    uint8_t index;
  };

  struct MemoryType{
    vlkHeapMemoryType type;
    uint32_t allocHandle;
  };

  //switch to small vec
  bcl::small_vector<MemoryType, 8> memory;
  bcl::small_vector<Heap, 4> heaps;
};

struct VlkGPUDescription{
  VkPhysicalDevice handle;
  bk::in_string<20> name;
  bk::bitset flags;
  vlkHeapStructure heaps;
};

}

