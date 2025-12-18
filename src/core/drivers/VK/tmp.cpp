#include "vk_core.h"
#include "vkinternal.h"
#include "vkentry.h"
#include "MemoryVK/MemoryVK.h"

using namespace juye::driver;

void VK::tCreateLightBuffers(){
  VkMemoryRequirements req{};
  vkcall(CreateVkBuffer(device, &ambientLightGpuBuffer, kMaxLights * sizeof(LightEntry), 
          VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT))
  vkGetBufferMemoryRequirements(device, ambientLightGpuBuffer, &req);
  vkcall(MemoryVK::Allocate(device, &ambientLightGpuMemory, req.size, _macosDeviceLocalFlag))
  vkcall(vkBindBufferMemory(device, ambientLightGpuBuffer, ambientLightGpuMemory, 0))

  vkcall(CreateVkBuffer(device, &directionalLightGpuBuffer, kMaxLights * sizeof(LightEntry), 
          VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT))
  vkGetBufferMemoryRequirements(device, directionalLightGpuBuffer, &req);
  vkcall(MemoryVK::Allocate(device, &directionalLightGpuMemory, req.size, _macosDeviceLocalFlag))
  vkcall(vkBindBufferMemory(device, directionalLightGpuBuffer, directionalLightGpuMemory, 0))
}
