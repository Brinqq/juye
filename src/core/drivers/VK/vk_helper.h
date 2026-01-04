#pragma once

#include "vkdefines.h"

#include <bcl/containers/vector.h>

// util functions and thin wrappers over the vulkan api to speed up development.


class GLFWwindow;

namespace vkh{

//platform specific
#if __APPLE__
  VkSurfaceKHR GetPlatformSurface(VkInstance instance, GLFWwindow* handle);
  void GetPlatformExtensions(bcl::small_vector<const char*>& vec);
#endif


#if _WIN32

#endif

#if __LINUX__

#endif


struct PipelineState{
  VkRenderPass renderpass;
  VkPipelineLayout layout;
};

enum VkhImageFlags{
  Multisampling = 0x0,
};


VkFormat GetCompatibleSurfaceFormat(VkPhysicalDevice gpu, VkSurfaceKHR surface);
VkColorSpaceKHR GetCompatibleSurfaceColorSpace(VkPhysicalDevice gpu, VkSurfaceKHR surface);
VkExtent2D GetCompatibleSurfaceExtent();

//create info helpers.
//



//vulkan objects creation helpers.
// 



VkPipeline CreatePipeline(VkDevice device, PipelineState& pipeline);
void DestroyPipeline(VkDevice device);

VkRenderPass CreateRenderpass(VkDevice device);
void DestroyRenderpass(VkDevice device);


VkImageView CreateImageView(VkDevice device, VkImage image, VkImageViewType dem, VkFormat format, VkImageAspectFlags aspect);
void DestroyImageView(VkDevice device, VkImageView view);

VkResult CreateBuffer(VkDevice device, VkBuffer* buf, size_t bytes, const VkBufferUsageFlags usage);
void DestroyBuffer(VkDevice device, VkBuffer buffer);

}//namespace vkh;

namespace juye{
  VkDeviceQueueCreateInfo vlkQueueInfo(uint32_t index, uint32_t count, float p);
  vlkHeapStructure vlkGenerateHeapStructure(VkPhysicalDevice gpu);
  void vlkGetGpu(VkInstance instance, uint32_t* pMaxGPUs, VlkGPUDescription* pGPUs);
  bool vlkCheckInstanceLayers(const bk::span<const char*>& span);
  bool vlkCheckInstanceExtensions(const char* pLayer, const bk::span<const char*>& span);
}
