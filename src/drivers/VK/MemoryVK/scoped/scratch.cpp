#include "scratch.h"
#include "../internal.h"
#include "bcl/memory/bkmemory.h"

using namespace vak;
namespace vak{
  extern vakDeviceContext gDeviceContext;
}

Error scScratch::Init(const HeapType type, VkMemoryPropertyFlags flags, VkAllocationCallbacks* allocator){

  uint64_t size = bk::AlignedSize(vak::GetHeapSize(type), gDeviceContext.pageGranularity);
  
   mCur = mStart;
   mEnd = mStart + size;

  int t = GetAvailableMemoryType(size, flags);
  if(t == -1){return VAK_OUT_OF_MEMORY;}

  VkMemoryAllocateInfo info{};
  info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.memoryTypeIndex = t;
  info.allocationSize = mEnd - mStart;
  VkResult result = vkAllocateMemory(gDeviceContext.device, &info, allocator, &mHandle);
  return VAK_SUCCESS;
};

Error scScratch::Resize(const HeapType type, VkMemoryPropertyFlags flags, VkAllocationCallbacks* allocator){
  Destroy(allocator);
  Init(type, flags, allocator);
  return VAK_SUCCESS;
}

void scScratch::Destroy(VkAllocationCallbacks* allocator){
  vkFreeMemory(gDeviceContext.device, mHandle, allocator);
}

Error scScratch::Allocate(uint64_t bytes, uint32_t alignment, Memory* mem){
  uint64_t size = bk::AlignedSize(bytes, alignment);
  if(size + mCur > mEnd){
    return VAK_OUT_OF_MEMORY;
  }
  
  mem->offset = mCur;
  mCur += size;
  return VAK_SUCCESS;
}

void scScratch::Reset(){
  mCur = mStart;
}

Error scScratch::Bind(Memory memory, VkImage image){
  VkResult result = vkBindImageMemory(gDeviceContext.device, image, mHandle, memory.offset);
  assert(result == VK_SUCCESS);
  return VAK_SUCCESS;
}

Error scScratch::Bind(Memory memory, VkBuffer buf){
  VkResult result = vkBindBufferMemory(gDeviceContext.device, buf, mHandle, memory.offset);
  assert(result == VK_SUCCESS);
  return VAK_SUCCESS;
}
