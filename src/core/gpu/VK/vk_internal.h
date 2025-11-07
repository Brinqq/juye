#pragma once

#include "vk_defines.h"
#include <utility>


namespace ivk{

struct CopyBufferOp{
  VkBuffer src;
  VkBuffer dst;
  VkDeviceSize srcOffset;
  VkDeviceSize dstOffset;
  VkDeviceSize bytes;
};

// Creation functions
VkResult CreateGraphicPipeline(VkDevice device, const PipelineShaders& shaders, VkRenderPass renderpass, VkPipeline* pipeline);

//runtime operations
void CopyBuffers(VkDevice device, VkCommandBuffer buf, CopyBufferOp* pCopyOp, uint32_t count);

//utils
std::pair<void*, size_t> CompileShaderSource(const char* path);

}
