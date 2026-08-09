#define __JUYE_METAL 1

#if __JUYE_METAL

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "base/global.h"
#include "drivers/gkhi/metal/metal.h"
#include "drivers/gkhi/metal/mtl_helpers.h"
#include "drivers/gkhi/metal/layer_adapter.h"
#include "drivers/gkhi/metal//futils.h"
#include "graphick/renderer/imst.h"
#include "graphick/renderer/adapter.h"

#include "bk/containers/bucket.h"

#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

#define MAX_FRAMES_IN_FLIGHT


// NOTE: Some things to think about

// API Facing:
// - Resource Pools(r)
//  
//

// Resource Pool:
// - Residency Set
// - Custom buffers
// - vertex/index pull

// Pipeline Managed components:
// - Shader
// - Argument Table
// - Attachments
// - Render Encoders

// Fixed State Components:
// - Samplers

// TBD:
// - Command Buffers:
//    * Only time will tell
//
// - Command Allocators:
//    * Who knows
//
// - Command Queues:
//    * *shrug*
//
// - Textures:
//   * Textures have very differing lifetime then standard buffers stored in pools
//    and might be cross referenced by resources in differing pools for example  pool x[object1] might
//    need to reference a texture in pool y. This is problematic when x lives longer then y.
//    find another way to represent this state;
//
// - transform data
//  * if we put this in pool and indexed it would require a model matrix for every model to keep the buffers aligned.
//  possible solution would be a transform buf for pool and a index pushed through a constant buffer for a draw.

extern void* query_main_display();



struct resource_pool{
  MTL::ResidencySet* set;

  bool check_pool(){
    return set != nullptr;
  }
};

#define as_buffer(h) static_cast<MTL::Buffer*>(h)
#define as_texture(h) static_cast<MTL::Texture*>(h)
#define as_resource_pool(h) static_cast<resource_pool*>(h)

MTL4::CommandQueue* cq;
MTL4::CommandBuffer* cb;
MTL4::CommandAllocator* ca[2];
MTL4::RenderCommandEncoder* rencoder;
MTL::Library* library;
MTL::SharedEvent* buffer_sync;
MTL4::Compiler* compiler;
MTL::SharedEvent* frame_sync;
MTL4::ArgumentTable* argument_table;
MTL::RenderPipelineState* pso;
MTL::Texture* depth_buf;
MTL::DepthStencilState* depth_state;
MTL::ResidencySet* residency_set;
uint64_t cur_frame = 0;


//TODO: Fix before supporting backend multihreading.
NS::Error* ns_error;

static MTL::SamplerState* sampler_state;

bk::bucket<resource_pool, 5> rpools;

using namespace juye;

int gdi_device::init_driver(){
  NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
  NS::Error* err = nullptr;

  window = query_main_display();

  if(!window){
    _juye_crashf("GPU driver unable to find a valid output device")
  }

  // Basic Handles
  device = MTL::CreateSystemDefaultDevice();

  if(!device){
    _juye_crashf("GPU driver failed to initialize!")
  }

  //require mtl4
  if(!device->supportsFamily(MTL::GPUFamilyMetal4)){
    _juye_crashf("Requires metal 4, update OSX version");
  }

  juye::LayerConfig cfg{window, device, MTL::PixelFormatBGRA8Unorm, 2, false};
  display_layer = (CA::MetalLayer*)juye::metal_layer_attach(cfg);


  cq = device->newMTL4CommandQueue();
  cb = device->newCommandBuffer();
  ca[0] = device->newCommandAllocator();
  
  // // -- Event
  frame_sync = device->newSharedEvent();
  buffer_sync = device->newSharedEvent();
  cur_frame = buffer_sync->signaledValue();
  
   MTL::Texture* back_buffer = display_layer->nextDrawable()->texture();
   depth_buf = generate_depth_buffer(device, back_buffer->width(), back_buffer->height(), MTL::PixelFormatDepth32Float);

   MTL::DepthStencilDescriptor* z_desc = MTL::DepthStencilDescriptor::alloc()->init()->autorelease();
   z_desc->setDepthCompareFunction(MTL::CompareFunctionLess);
   z_desc->setDepthWriteEnabled(1);
   depth_state = device->newDepthStencilState(z_desc);


   // TODO: add functionality for growing and also auto fill each one to a identity matrix
   transform_buf = device->newBuffer(64 * 4, MTL::ResourceStorageModeShared);


   // NOTE: Careful with alingment issues when appending to the buffer.
  llcb = device->newBuffer(sizeof(mtl_llcb), MTL::ResourceStorageModeShared);
  memset(llcb->contents(), 0, sizeof(mtl_llcb));

  MTL4::ArgumentTableDescriptor* argument_desc = MTL4::ArgumentTableDescriptor::alloc()->init()->autorelease();
  argument_desc->setMaxBufferBindCount(31);
  argument_desc->setMaxTextureBindCount(5);
  argument_desc->setMaxSamplerStateBindCount(1);
  argument_table = device->newArgumentTable(argument_desc, &err);
   
// -- Pipeline state
  auto* pipeline_desc = MTL4::RenderPipelineDescriptor::alloc()->init()->autorelease();


  auto* vertex_desc = MTL::VertexDescriptor::alloc()->init()->autorelease();
  juye::mtl_append_vertex_attribute(*vertex_desc, 0, 0, 28, MTL::VertexFormatFloat3);
  juye::mtl_append_vertex_attribute(*vertex_desc, 1, sizeof(float) * 3, 28, MTL::VertexFormatFloat3);
  juye::mtl_append_vertex_attribute(*vertex_desc, 2, sizeof(float) * 6, 28, MTL::VertexFormatFloat2);
  vertex_desc->layouts()->object(28)->setStride(8 * sizeof(float));
  vertex_desc->layouts()->object(28)->setStepFunction(MTL::VertexStepFunctionPerVertex);
  pipeline_desc->setVertexDescriptor(vertex_desc);

  pipeline_desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm); 

//   // - Shader
  library = device->newLibrary(MTLSTR("/Users/brinq/.dev/projects/solar-sim/build/bin/data/triangle.metallib"),&err);
  if (!library) {
    printf("newLibrary failed: %s\n",
           err ? err->localizedDescription()->utf8String() : "(no error object)");
    abort();
}

  MTL4::LibraryFunctionDescriptor* vs_function_desc = MTL4::LibraryFunctionDescriptor::alloc()->init()->autorelease();
  vs_function_desc->setLibrary(library);
  vs_function_desc->setName(MTLSTR("vertex_main"));
  pipeline_desc->setVertexFunctionDescriptor(vs_function_desc);
  MTL4::LibraryFunctionDescriptor* ps_function_desc = MTL4::LibraryFunctionDescriptor::alloc()->init()->autorelease();
  ps_function_desc->setLibrary(library);
  ps_function_desc->setName(MTLSTR("pixel_main"));
  pipeline_desc->setFragmentFunctionDescriptor(ps_function_desc);
  auto* desc = MTL4::CompilerDescriptor::alloc()->init()->autorelease();
  compiler = device->newCompiler(desc, &err);
  pso = compiler->newRenderPipelineState(pipeline_desc, nullptr, &err);

  if (!pso) {
      _juye_crashf("PSO failed: %s", err->localizedDescription()->utf8String());
  }

  //samplers def

  // mtl_generate_sampler();

  auto* x = MTL::SamplerDescriptor::alloc()->init()->autorelease();
  x->setNormalizedCoordinates(true);
  x->setRAddressMode(MTL::SamplerAddressModeClampToEdge);
  x->setSAddressMode(MTL::SamplerAddressModeClampToEdge);
  x->setTAddressMode(MTL::SamplerAddressModeClampToEdge);
  x->setBorderColor(MTL::SamplerBorderColorTransparentBlack);
  x->setMinFilter(MTL::SamplerMinMagFilterLinear);
  x->setMagFilter(MTL::SamplerMinMagFilterLinear);
  x->setMaxAnisotropy(1);
  x->setMipFilter(MTL::SamplerMipFilterLinear);
  x->setSupportArgumentBuffers(true);
  device->newSamplerState(x);

  auto rdesc = MTL::ResidencySetDescriptor::alloc()->init()->autorelease();
  residency_set = device->newResidencySet(rdesc, &err);
  residency_set->addAllocation(llcb);
  residency_set->addAllocation(transform_buf);
  residency_set->commit();

  pool->release();
  return 1;
}

gdi_memory gdi_device::allocate_texture(gdi_rpool pool, size_t w, size_t h, uint32_t mips){
  resource_pool* p  = as_resource_pool(pool);

 if(!p->check_pool()){
    _juye_crashf("Attempted to allocate texture with invalid resource pool")
 };

  MTL::TextureDescriptor* desc = MTL::TextureDescriptor::alloc()->init();
  desc->setPixelFormat(MTL::PixelFormat::PixelFormatRGBA8Unorm);
  desc->setArrayLength(1);
  desc->setWidth(w);
  desc->setHeight(h);
  desc->setDepth(1);
  desc->setResourceOptions(MTL::ResourceStorageModeShared);
  desc->setMipmapLevelCount(mips);
  desc->setSampleCount(1);
  desc->setUsage(MTL::TextureUsageShaderRead);
  MTL::Texture* ret = device->newTexture(desc);
  desc->release();
  p->set->addAllocation(ret);
  return ret;
}


gdi_memory gdi_device::allocate_buffer(gdi_rpool pool, size_t n_bytes, const uint64_t flags){
  resource_pool* p = as_resource_pool(pool);

  if(!p->check_pool()){
    _juye_crashf("Attempted to allocate buffer with a invalid resource pool.")
  }

  MTL::Buffer* mem = device->newBuffer(n_bytes, MTL::StorageModeShared);
  p->set->addAllocation(mem);
  return mem;
}

gdi_transform gdi_device::generate_transform(){
  size_t cur = transform_offset;
  transform_offset++;
  return cur;
}


void gdi_device::write_transform(gdi_transform t, void* src){
  void* dst = static_cast<uint8_t*>(transform_buf->contents()) + (t * 64);
  memcpy(dst, src, 64);
}


void set_constant_buffer(gdi_memory buf){
   
}

gdi_rpool gdi_device::allocate_resource_pool(){
  resource_pool* ret =  &rpools.construct();
  MTL::ResidencySetDescriptor* desc = MTL::ResidencySetDescriptor::alloc()->init();
  MTL::ResidencySet* set = device->newResidencySet(desc, &ns_error);
  ret->set = set;
  desc->release();
  return ret;
}

void gdi_device::set_viewport(float w, float h, float n, float f, float x, float y){
  viewport = MTL::Viewport{x, y, w, h, n,f};
}

void gdi_device::set_projection(const float* v, const float* p){
  memcpy(llcb->contents(), v, sizeof(float) * 16);
  memcpy((uint8_t*)llcb->contents() + 64, p, sizeof(float) * 16);
}

void gdi_device::write_texture(void* src, gdi_memory mem, size_t width, size_t height, uint32_t mip){
  MTL::Region reg = MTL::Region::Make2D(0, 0, width, height);
  static_cast<MTL::Texture*>(mem)->replaceRegion(reg, mip, 0, src, width * 4, (width * 4) * height);
}

void gdi_device::write_buffer(void* src, gdi_memory dst, size_t n_bytes, size_t offset){
  memcpy(static_cast<MTL::Buffer*>(dst)->contents(), src, n_bytes);
}

// This function sets redundant state and is unideal for anything other
// than iteration speed and testing.
void gdi_device::execute_single_draw(){
  // NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
  // rencoder->setRenderPipelineState(pso);
  // rencoder->setArgumentTable(argument_table, MTL::RenderStageVertex | MTL::RenderStageFragment);
  // rencoder->drawIndexedPrimitives();
};

void gdi_device::next_frame(){
  // ca[0]->reset();
  // cb->beginCommandBuffer(ca[0]);
  // auto* drawable = display_layer->nextDrawable();
}

void gdi_device::submit_frame(){
  // TODO: add support for multiple frame througput;
  next_frame();
}

void gdi_device::commit_resource_pool(gdi_rpool pool){
  resource_pool* p = as_resource_pool(pool);
  if(!p->check_pool()){
    _juye_crashf("Attempted to commit a invalid resource pool")
  };

  p->set->commit();
}


uint64_t f = 0;


void gdi_device::dummy_draw(void* vertices, void* indices, size_t n_indices, gdi_transform transform, gdi_memory user_cbuf,
gdi_memory texture, gdi_rpool u_pool){
  NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
  NS::Error* err = nullptr;

  //this is fine since we stall at the end of frame for now.
  auto& command_allocator =  ca[0];
  command_allocator->reset(); 
  cb->beginCommandBuffer(command_allocator);

  auto* drawable = display_layer->nextDrawable();
  float w =drawable->texture()->width();
  float h = drawable->texture()->height();

  auto* dd = MTL4::RenderPassDescriptor::alloc()->init();
  auto* att = dd->colorAttachments()->object(0);
  att->setTexture(drawable->texture());
  att->setLoadAction(MTL::LoadActionClear);
  att->setStoreAction(MTL::StoreActionStore);
  att->setClearColor(MTL::ClearColor(0.10588, 0.12549, 0.12941, 1.0));
  auto* zwrite = dd->depthAttachment();
  zwrite->setTexture(depth_buf);
  zwrite->setLoadAction(MTL::LoadActionClear);
  zwrite->setStoreAction(MTL::StoreActionDontCare);
  zwrite->setClearDepth(1);
  MTL4::RenderCommandEncoder* renderpass_encoder = cb->renderCommandEncoder(dd);

  set_viewport(w, h, 0, 1, 0, 0);
  argument_table->setAddress(static_cast<MTL::Buffer*>(vertices)->gpuAddress(), 28);
  argument_table->setAddress(llcb->gpuAddress(), 0);
  argument_table->setAddress(transform_buf->gpuAddress(), 1);
  argument_table->setAddress(static_cast<MTL::Buffer*>(user_cbuf)->gpuAddress(), 2);
  argument_table->setTexture(static_cast<MTL::Texture*>(texture)->gpuResourceID(), 0);
  argument_table->setSamplerState(sampler_state->gpuResourceID(), 0);

  cq->addResidencySet(residency_set);
  cq->addResidencySet(static_cast<resource_pool*>(u_pool)->set);
  cq->addResidencySet(display_layer->residencySet());

  renderpass_encoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
  renderpass_encoder->setCullMode(MTL::CullModeBack);
  renderpass_encoder->setViewport(viewport);
  renderpass_encoder->setRenderPipelineState(pso);
  renderpass_encoder->setArgumentTable(argument_table, MTL::RenderStageVertex | MTL::RenderStageFragment);
  renderpass_encoder->setDepthStencilState(depth_state);
  renderpass_encoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, n_indices, MTL::IndexTypeUInt16,
                                            static_cast<MTL::Buffer*>(indices)->gpuAddress(), 2 * n_indices  
  );
  
  cq->wait(drawable);
  renderpass_encoder->endEncoding();
  cb->endCommandBuffer();
  cq->commit(&cb, 1);
  cq->signalDrawable(drawable);
  drawable->present();
  cq->signalEvent(frame_sync, ++f);
  frame_sync->waitUntilSignaledValue(f, 1000);

  cq->removeResidencySet(residency_set);
  cq->removeResidencySet(static_cast<resource_pool*>(u_pool)->set);
  cq->removeResidencySet(display_layer->residencySet());

  dd->release();
  pool->release();
}

#endif
