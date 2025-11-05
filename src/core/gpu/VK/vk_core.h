#pragma once

#include "vk_defines.h"
#include <vector>

//tmp
#include "gfx/prefabs.h"
//---------

struct Device;

class VK{
static constexpr uint64_t _StagingBufferSize = 1000000;

static constexpr int _MacosDeviceLocalFlag = 0;
static constexpr int _MacosHostAccessFlag = 1;

    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice gpu;

    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkImageView swapchainViews[2];
    VkFramebuffer framebuffers[2];
    VkRenderPass mainRenderpass;

    VkCommandPool graphicsPool;
    VkCommandPool transferPool;

    VkCommandBuffer mainCommandBuffer;
    VkFence mainFence;

    VkQueue graphicQueue;
    VkQueue transferQueue;

    std::vector<QueueFamily> queueFamilies;
    std::pair<VkBuffer, VkDeviceMemory> stagingBuffers[2];

    struct{
      const ssf::prefabs::StandardCube<uint16_t> data;
      VkBuffer vbo;
      VkDeviceMemory vHandle;
      uint64_t vDataSize;
      VkBuffer ibo;
      VkDeviceMemory iHandle;
      uint64_t iDataSize;
      

    }_TmpCube;

public:
  int Init();
  void Destroy();

  int CreateComputeState();
  int DestroyComputeState();

  int CreateGraphicsState(Device& device);
  int DestroyGraphicsState();

  void Draw();
  void TestTriangle();

};//class VK
