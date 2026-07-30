#pragma once
#include "../MemoryVK.h"

namespace vak{

class scScratch{
private:
  static constexpr uint64_t mStart = 0;
  uint64_t mCur;
  uint64_t mEnd;
  VkDeviceMemory mHandle;

  struct MemoryHandle{
  private:
    uint64_t offset;
    friend scScratch;
  };

public:
  typedef MemoryHandle Memory;
  Error Init(const HeapType type, VkMemoryPropertyFlags flags, VkAllocationCallbacks* allocator);
  Error Resize(const HeapType type, VkMemoryPropertyFlags flags, VkAllocationCallbacks* allocator);
  void Destroy(VkAllocationCallbacks* allocator);
  Error Allocate(uint64_t bytes, uint32_t alignment, Memory* mem);
  void Reset();
Error Bind(Memory memory, VkImage image);
Error Bind(Memory memory, VkBuffer buf);

};

}
