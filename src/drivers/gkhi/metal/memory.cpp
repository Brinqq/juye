#include "Metal/MTLHeap.hpp"
#include "drivers/gkhi/interface.h"
#include "drivers/gkhi/types.h"

using namespace juye;

gdi_memory gdi_device::allocate_memory(const gdi_allocation_desc& memory){
  uint64_t storage = 0;

  switch(memory.type){
  case gdi_memory_type::local:
        storage = storage | MTL::ResourceStorageModePrivate;
      break;

      case gdi_memory_type::host_write:
        storage = storage | MTL::ResourceStorageModeShared;
        storage = storage | MTL::ResourceCPUCacheModeWriteCombined;
      break;
      
      case gdi_memory_type::host_readback:
        storage = storage | MTL::ResourceStorageModeShared;
      break;
  }

  auto d_heap = MTL::HeapDescriptor::alloc()->init();
  d_heap->setSize(memory.n_bytes);
  d_heap->setResourceOptions(storage);
  d_heap->setType(MTL::HeapTypePlacement);
  d_heap->setSparsePageSize(MTL::SparsePageSize16);
  MTL::Heap* heap = device->newHeap(d_heap);
  d_heap->release();
  set->addAllocation(heap);
  return heap;
}

void gdi_device::deallocate_memory(gdi_memory mem){

}
