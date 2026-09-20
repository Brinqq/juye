#pragma once

#include "bcl/containers/span.h"
#include "bcl/containers/vector.h"
#include "bcl/containers/bitset.h"
#include "bcl/containers/string.h"
#include "bcl/containers/cache.h"

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan/vulkan.h"
#include "vk_debug.h"

static constexpr int kMaxMemoryTypes = 10;
static constexpr int kBackBufferMax = 2;

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
  VkImageView* view;
};


class vlkSwapChain{
public:

  VkSwapchainKHR mHandle;
  VkImage mImages[kBackBufferMax];
  VkImageView mViews[kBackBufferMax];
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

