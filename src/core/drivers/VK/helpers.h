#pragma once

#include <vulkan/vulkan.h>

namespace juye{

VkFramebuffer vlkCreateFramebuffer(VkDevice device, VkRenderPass rpass, const VkExtent2D& extent, const VkImageView* pViews, uint32_t count, const VkAllocationCallbacks* allocator);
VkSampler vlkCreateSampler(VkDevice device, VkSamplerAddressMode mode, bool aniso, float ansioMax, uint32_t minLod, uint32_t maxLod, const VkAllocationCallbacks* allocator);
void vlkDestroySamplers(VkDevice device, VkSampler* samplers, uint32_t count, const VkAllocationCallbacks* allocator);
VkDescriptorSet vlkAllocateDescriptorSets(VkDevice device, const VkDescriptorPool& pool, const VkDescriptorSetLayout* pLayouts, uint32_t count);
void vlkDestoryDescriptorSets(VkDevice device, const VkDescriptorPool& pool, VkDescriptorSet* pSets, uint32_t count);

}
