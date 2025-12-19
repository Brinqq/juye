#pragma once
#include "vkdefines.h"

#include <array>
#include <vector>

namespace juye::driver{

enum ResourceType{
  ResourceStorage,
  ResourceUniform
};


class RenderPassBuilder{
private:
  static constexpr int kMaxAttachments = 5;
  static constexpr int kMaxSubpasses = 5;

public:
  enum AttachmentUsageType{
    AttachmentUsageRead,
    AttachmentUsageWrite,
  };

  enum AttachmentCreateFlags{
    AttachmentCreateNone = 0x0,
    AttachmentCreateClearOnLoad = 0x1,
    AttachmentCreateKeepOnLoad = 0x4,
    AttachmentCreateKeepOnStore = 0x8,
    AttachmentCreateClearOnStore = 0x8,
  };
private:

  struct Subpass{
    VkAttachmentReference write[kMaxAttachments];
    int numWriteAttachmets;
    VkAttachmentReference read[kMaxAttachments];
    int numReadAttachmets;
    uint32_t preserve[kMaxAttachments];
    int numPreserveAttachmets;
  };

  struct Attachment{
    VkFormat format;
    VkImageLayout layout;
  };

  std::array<VkAttachmentDescription, kMaxAttachments> attachments;
  std::array<VkSubpassDescription, kMaxSubpasses> subpassHandles;
  std::array<Subpass, kMaxSubpasses> subpasses;

  int numSubpasses  = 0;
  int numAttachments  = 0;
  bool hasDepthBuf = false;
  VkAttachmentReference depthRef;
public:


  RenderPassBuilder AddDepthAttachment(VkFormat format);
  RenderPassBuilder AddAttachment(const VkFormat format,const VkImageLayout initialLayout, 
                    const VkImageLayout gpuReadyLayout, uint32_t flags);

  RenderPassBuilder& BeginSubpass();
  RenderPassBuilder& SetReadAttachment(uint32_t index);
  RenderPassBuilder& SetWriteAttachment(uint32_t index);
  RenderPassBuilder& SetPreserveAttachment(uint32_t index);
  RenderPassBuilder& EndSubpass();
  RenderPassBuilder& Reset();

  VkRenderPass Build(VkDevice device);

};

class DescriptorSetLayoutBuilder{
private:
  static constexpr int kMaxBindings = 5;
  VkDescriptorSetLayoutBinding bindings[kMaxBindings];
  uint32_t count;
public:
  DescriptorSetLayoutBuilder& AddBinding(uint32_t idx, uint32_t batch, const VkDescriptorType type, const VkShaderStageFlags stages) noexcept;
  VkDescriptorSetLayout Build(VkDevice device, VkAllocationCallbacks* allocator) noexcept;

};// DescriptorSetLayoutBuilder
  

VkResult AllocateVkDescriptorSets(VkDevice device, const VkDescriptorPool& pool, const VkDescriptorSetLayout* pLayouts, 
         uint32_t count, VkDescriptorSet* pSets);


  
}// namespace juye::driver

