#pragma once

#include "vulkan/vulkan.h"

namespace MemoryVK{

VkResult Allocate(VkDevice device, VkDeviceMemory& handle, VkDeviceSize bytes);

}

