#include "base/global.h"

#include "drivers/gkhi/interface.h"

#include "futils.h"


using namespace juye;

gdi_resource gdi_device::create_buffer(gdi_memory mem, const gdi_buffer_desc& d_buf, size_t offset){
  MTL::Heap* heap = as_heap(mem);
  MTL::Buffer* buf  = heap->newBuffer(d_buf.n_bytes, heap->resourceOptions(), offset);
  _juye_assert(buf)
  return buf;
}

void gdi_device::destory_buffer(gdi_resource buf){
  static_cast<MTL::Buffer*>(buf)->release();
}

gdi_resource gdi_device::create_texture(gdi_memory mem, const gdi_texture_desc& tex, size_t offset){
  MTL::TextureDescriptor* desc = MTL::TextureDescriptor::alloc()->init();
  desc->setPixelFormat(MTL::PixelFormat::PixelFormatRGBA8Unorm);
  desc->setArrayLength(tex.array);
  desc->setWidth(tex.width);
  desc->setHeight(tex.height);
  desc->setDepth(tex.depth);
  desc->setResourceOptions(as_heap(mem)->resourceOptions());
  desc->setMipmapLevelCount(tex.mips);
  desc->setSampleCount(1);
  desc->setUsage(MTL::TextureUsageShaderRead);
  MTL::Texture* ret = device->newTexture(desc);
  desc->release();
  set->addAllocation(ret);
  return ret;
}

void gdi_device::destory_texture(){

}

void* gdi_device::map_resource(gdi_resource resource, const gdi_resource_type type){
  switch(type){
    case gdi_resource_type::buffer:
      return as_buffer(resource)->contents();

    case gdi_resource_type::texture:
      return as_texture(resource)->buffer()->contents();

      default:
        _juye_crashf("Attempted to map memory with invalid resource type.");
  }

  return nullptr;


}

void gdi_device::unmap_resource(gdi_resource resource, const gdi_resource_type type){
}


gdi_texture gdi_device::get_display_texture(){
}

