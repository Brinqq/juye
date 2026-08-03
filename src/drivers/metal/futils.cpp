#include "futils.h"

MTL::Texture* generate_depth_buffer(MTL::Device* device, size_t w, size_t h, MTL::PixelFormat format){
  
  MTL::TextureDescriptor* desc = MTL::TextureDescriptor::alloc()->init();
  desc->setUsage(MTL::TextureUsageRenderTarget);
  desc->setTextureType(MTL::TextureType2DArray);
  desc->setWidth(w);
  desc->setHeight(h);
  desc->setPixelFormat(format);

  desc->setDepth(1);
  desc->setStorageMode(MTL::StorageModeMemoryless);
  MTL::Texture* ret = device->newTexture(desc);
  desc->release();
  return ret;
}

