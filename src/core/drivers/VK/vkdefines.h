#pragma once

#include "vulkan/vulkan.h"
#include "vk_debug.h"

#include "bcl/containers/span.h"
#include "bcl/containers/vector.h"
#include "bcl/containers/bitset.h"
#include "bcl/containers/string.h"

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
enum vlkHeapBitFlags{
  HeapVRAMBit = 0x0,
  HeapCPUMappedBit = 0x1,
};

enum vlkGPUBitFlags{
  GpuDiscreteBit
};

struct vlkGPUHeapLayout{

};

struct VlkGPUDescription{
  VkPhysicalDevice handle;
  bk::in_string<20> name;
  bk::bitset flags;
  bcl::small_vector<vlkGPUHeapLayout, 5> heaps;
};


}

