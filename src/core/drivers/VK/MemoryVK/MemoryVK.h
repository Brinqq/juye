#pragma once

#include "vulkan/vulkan.h"

namespace MemoryVK{

VkResult Allocate(VkDevice device, VkDeviceMemory* handle, VkDeviceSize bytes, uint32_t type);
void Deallocate(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* allocator);


template<typename _Type, size_t _MaxSize>
class TypedScratchBuffer{
private:
  VkBuffer buffer;
  VkDeviceMemory memory;
  size_t bytes;
  size_t offset;
  size_t stride;
public:
  size_t Allocate(){
    size_t index = offset + stride;

    if(index >= bytes){
      //error
    };

    offset = index;
    return offset;
  }

  void Reset(){
    offset = 0;
  }

  void Write(VkDevice device, const _Type& pData){
  }


  TypedScratchBuffer(VkDevice device){}
  TypedScratchBuffer(VkDevice device, int alignment){}
  ~TypedScratchBuffer(){}
  TypedScratchBuffer() = delete;
  TypedScratchBuffer(const TypedScratchBuffer& rhs) = delete;
  TypedScratchBuffer(TypedScratchBuffer&& rhs) = delete;
};

}
