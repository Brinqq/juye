#pragma once

#include "bcl/containers/span.h"
#include "bcl/containers/vector.h"
#include "bcl/containers/bitset.h"
#include "bcl/containers/string.h"
#include "bcl/containers/cache.h"


#if _WIN32
#endif

#include "vulkan/vulkan.h"
#include "vk_debug.h"

static constexpr int kMaxMemoryTypes = 10;

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

struct vlkHeapTracker{

};


struct VlkGPUDescription{
private:
public:
  VkPhysicalDevice handle;
  bk::in_string<20> name;
  bk::bitset flags;
};


struct vlkDepthBuffer{
  VkImage image;
  VkFormat format;

};

class vlkResourcePA{
public:
  vlkHeapTracker* mHeapTracker;
  typedef void* CleanupHandle;

  int Create(vlkHeapTracker* tracker, uint64_t bytes);
  void Destory();
  void Reconfigure();
};

class vlkSwapChain{
  static constexpr int kBackBufferMax = 2;
public:

  VkSwapchainKHR mHandle;
  VkImage mImages[kBackBufferMax];
  uint32_t mBackBufferCount = 2;
  uint8_t mCurrentBuf;
  VkExtent2D mExtent;
  VkFormat mFormat;
  VkColorSpaceKHR mColorspace;

  int Create(VkDevice device, VkPhysicalDevice gpu, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator);
  void Destroy(VkDevice device, VkAllocationCallbacks* pAllocator);
  void Resize(VkSurfaceKHR surface);
};

}

