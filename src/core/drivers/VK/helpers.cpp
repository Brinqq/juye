#include "helpers.h"
#include "vk_debug.h"

namespace juye{

VkFramebuffer vlkCreateFramebuffer(VkDevice device, VkRenderPass rpass, const VkExtent2D& extent, const VkImageView* pViews, 
        uint32_t count, const VkAllocationCallbacks* allocator){
  VkFramebuffer ret = VK_NULL_HANDLE;

  VkFramebufferCreateInfo i{
    VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    nullptr,
    0,
    rpass,
    count,
    pViews,
    extent.width,
    extent.height,
    1
  };

  vkcall(vkCreateFramebuffer(device, &i, allocator, &ret))
  return ret;
}

VkSampler vlkCreateSampler(VkDevice device, VkSamplerAddressMode mode, bool aniso, float ansioMax,
                       uint32_t minLod, uint32_t maxLod, const VkAllocationCallbacks* allocator){

    VkSampler ret = VK_NULL_HANDLE;
    VkSamplerCreateInfo cSampler;
    cSampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    cSampler.pNext = nullptr;
    cSampler.flags = 0;
    cSampler.magFilter = VK_FILTER_LINEAR;
    cSampler.minFilter = VK_FILTER_LINEAR;
    cSampler.addressModeU = mode;
    cSampler.addressModeV = mode;
    cSampler.addressModeW = mode;
    cSampler.compareEnable = VK_FALSE;
    cSampler.compareOp = VK_COMPARE_OP_ALWAYS;
    cSampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    cSampler.mipLodBias = 0;
    cSampler.minLod = minLod;
    cSampler.maxLod = maxLod;
    cSampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    cSampler.unnormalizedCoordinates = VK_FALSE;

    if(aniso){
      cSampler.anisotropyEnable = VK_FALSE;
      cSampler.maxAnisotropy = ansioMax;
    }else{
      cSampler.anisotropyEnable = VK_TRUE;
      cSampler.maxAnisotropy = 0;
    }

    vkcall(vkCreateSampler(device, &cSampler, allocator, &ret))
    return ret;
}

void vlkDestroySamplers(VkDevice device, VkSampler* samplers, uint32_t count, const VkAllocationCallbacks* allocator){
  for(int i = 0; i < count; ++i){
    vkDestroySampler(device, samplers[i], allocator);
  }
}


VkDescriptorSet vlkAllocateDescriptorSets(VkDevice device, const VkDescriptorPool& pool, 
              const VkDescriptorSetLayout* pLayouts, uint32_t count){
  VkDescriptorSet ret = VK_NULL_HANDLE;
  VkDescriptorSetAllocateInfo info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, 0, pool, count, pLayouts};
  vkcall(vkAllocateDescriptorSets(device, &info, &ret))
  return ret;
}

void vlkDestoryDescriptorSets(VkDevice device, const VkDescriptorPool& pool, VkDescriptorSet* pSets, uint32_t count){
    vkFreeDescriptorSets(device, pool, count, pSets);
}

}
