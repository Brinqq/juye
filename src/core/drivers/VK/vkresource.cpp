#include "vkresource.h"
#include "core/global.h"

using namespace juye::driver;


  RenderPassBuilder RenderPassBuilder::AddDepthAttachment(VkFormat format){
    hasDepthBuf = true;

    VkAttachmentDescription desc;
    desc.flags = 0;
    desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    desc.format = format;
    desc.samples = VK_SAMPLE_COUNT_1_BIT;
    desc.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[numAttachments] = desc;
    depthRef = VkAttachmentReference{static_cast<uint32_t>(numAttachments),  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
    numAttachments++;
    return *this;
  }

  RenderPassBuilder RenderPassBuilder::AddAttachment(const VkFormat format,const VkImageLayout initialLayout, 
                    const VkImageLayout gpuReadyLayout, uint32_t flags){

    VkAttachmentDescription desc;
    desc.flags = 0;
    desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc.finalLayout = gpuReadyLayout;
    desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    desc.format = format;
    desc.samples = VK_SAMPLE_COUNT_1_BIT;
    desc.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    
    if(flags & AttachmentCreateClearOnLoad){
      desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    }else if(flags & AttachmentCreateKeepOnLoad){
      desc.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    }

    if(flags & AttachmentCreateKeepOnStore){ desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE; }

    attachments[numAttachments] = desc;
    numAttachments++;

    return *this;
  }

  RenderPassBuilder& RenderPassBuilder::BeginSubpass(){
    auto& pass = subpasses.at(numSubpasses);
    auto& desc = subpassHandles.at(numSubpasses);
    desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    desc.flags = 0;
    desc.pResolveAttachments = nullptr;
    desc.pDepthStencilAttachment = nullptr;
    pass.numWriteAttachmets = 0;
    pass.numReadAttachmets = 0;
    pass.numPreserveAttachmets = 0;

    if(hasDepthBuf){
      desc.pDepthStencilAttachment = &depthRef;
    }

    return *this;
  }

  RenderPassBuilder& RenderPassBuilder::SetReadAttachment(uint32_t index){
    auto& pass = subpasses.at(numSubpasses);
    auto& attachment = attachments[index];
    VkAttachmentReference ref{index, attachment.initialLayout};
    pass.read[pass.numReadAttachmets] = ref;
    pass.numReadAttachmets++;
    return *this;
  }

  RenderPassBuilder& RenderPassBuilder::SetWriteAttachment(uint32_t index){
    auto& pass = subpasses.at(numSubpasses);
    auto& attachment = attachments[index];
    VkAttachmentReference ref{index, attachment.initialLayout};
    pass.write[pass.numWriteAttachmets] = ref;
    pass.numWriteAttachmets++;
    return *this;
  }

  
  RenderPassBuilder& RenderPassBuilder::SetPreserveAttachment(uint32_t index){
    auto& pass = subpasses.at(numSubpasses);
    auto& attachment = attachments[index];
    VkAttachmentReference ref{index, attachment.initialLayout};
    pass.preserve[pass.numPreserveAttachmets] = index;
    pass.numPreserveAttachmets++;
    return *this;
  }

  RenderPassBuilder& RenderPassBuilder::EndSubpass(){
    auto& pass = subpasses.at(numSubpasses);
    auto& desc = subpassHandles.at(numSubpasses);
    desc.pInputAttachments = pass.read;
    desc.pColorAttachments = pass.write;
    desc.pPreserveAttachments = pass.preserve;

    desc.inputAttachmentCount = pass.numReadAttachmets;
    desc.colorAttachmentCount = pass.numWriteAttachmets;
    desc.preserveAttachmentCount = pass.numPreserveAttachmets;
    numSubpasses++;

    return *this;
  }

  RenderPassBuilder& RenderPassBuilder::Reset(){
    return *this;
  }

  VkRenderPass RenderPassBuilder::Build(VkDevice device){
    
  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  
    VkRenderPass ret;
    VkRenderPassCreateInfo c;

    c.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    c.pNext = nullptr;
    c.flags = 0;
    c.attachmentCount = numAttachments;
    c.pAttachments = attachments.data();
    c.subpassCount = numSubpasses;
    c.pSubpasses = subpassHandles.data();
    c.dependencyCount = 1;
    c.pDependencies = &dependency;

    vkcall(vkCreateRenderPass(device, &c, nullptr, &ret))
    return ret;

  }


DescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::AddBinding(uint32_t idx, uint32_t batch,
                          const VkDescriptorType type, const VkShaderStageFlags stages) noexcept{

  VkDescriptorSetLayoutBinding binding{};
  binding.binding = idx;
  binding.descriptorCount = batch;
  binding.descriptorType = type;
  binding.pImmutableSamplers = nullptr;
  binding.stageFlags = stages;

  bindings[count] = binding;
  count++;

  return *this;
}

VkDescriptorSetLayout DescriptorSetLayoutBuilder::Build(VkDevice device, VkAllocationCallbacks* allocator) noexcept{
  VkDescriptorSetLayout ret;

  VkDescriptorSetLayoutCreateInfo layout{
   VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    nullptr,
    0, // configurable maybe?
    count,
    bindings,
  };

  vkcall(vkCreateDescriptorSetLayout(device, &layout, allocator, &ret))
  return ret;
}


  VkResult juye::driver::AllocateVkDescriptorSets(VkDevice device, const VkDescriptorPool& pool, 
            const VkDescriptorSetLayout* pLayouts, uint32_t count, VkDescriptorSet* pSets){

    VkDescriptorSetAllocateInfo info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, 0, pool, count, pLayouts };
    return vkAllocateDescriptorSets(device, &info, pSets);
  }




