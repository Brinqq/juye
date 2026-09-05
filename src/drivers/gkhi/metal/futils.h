#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

MTL::Texture* generate_depth_buffer(MTL::Device* device, size_t w, size_t h, MTL::PixelFormat format);

#define as_buffer(h) static_cast<MTL::Buffer*>(h)
#define as_texture(h) static_cast<MTL::Texture*>(h)
#define as_heap(h) static_cast<MTL::Heap*>(h)
#define as_resource_pool(h) static_cast<resource_pool*>(h)

