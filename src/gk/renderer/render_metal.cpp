#define __JUYE_METAL 1

#if __JUYE_METAL

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "base/global.h"

#include "drivers/metal/metal.h"
#include "drivers/metal/mtl_helpers.h"
#include "drivers/metal/layer_adapter.h"
#include "drivers/metal//futils.h"

#include "gk/renderer/imst.h"
#include "gk/renderer/adapter.h"

#include "bk/containers/bucket.h"

#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

#define MAX_FRAMES_IN_FLIGHT


// NOTE: Some things to think about
//
// - Command buffers require a cpu allocation and are not pooled like vulkan
// consider retaining them over frames.
//
// - Do encoders previous state presist?
// 

extern void* query_main_display();

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
uint64_t cur_frame = 0;

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
   z_desc->setDepthCompareFunction(MTL::CompareFunctionGreater);
   z_desc->setDepthWriteEnabled(1);
   depth_state = device->newDepthStencilState(z_desc);


   // NOTE: Careful with alingment issues when appending to the buffer.
  llcb = device->newBuffer(sizeof(mtl_llcb), MTL::ResourceStorageModeShared);
  memset(llcb->contents(), 0, sizeof(mtl_llcb));
  

  MTL4::ArgumentTableDescriptor* argument_desc = MTL4::ArgumentTableDescriptor::alloc()->init()->autorelease();
  argument_desc->setMaxBufferBindCount(32);
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
  library = device->newLibrary(MTLSTR("build/bin/data/tri.metallib"),&err);
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
  pool->release();
  return 1;
}

gdi_memory gdi_device::allocate_memory(void* fill, size_t n_bytes, const gdi_memory_flags flags){
  void* mem = nullptr;

  if(GDI_MEMORY_LINEAR_BIT){
    mem = device->newBuffer(n_bytes, MTL::ResourceStorageModeShared);
  }

  if(fill){}

  return mem;
}

void gdi_device::set_viewport(float w, float h, float n, float f, float x, float y){
  viewport = MTL::Viewport{x, y, w, h, n,f};
}

void gdi_device::set_projection(const float* v, const float* p){
  memcpy(llcb->contents(), v, sizeof(float) * 16);
  memcpy((uint8_t*)llcb->contents() + 64, p, sizeof(float) * 16);
}


void gdi_device::write_memory(void* src, gdi_memory dst, size_t n_bytes, size_t offset){
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


uint64_t f = 0;


void gdi_device::dummy_draw(void* vertices, void* indices, size_t n_indices){
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
  att->setClearColor(MTL::ClearColor(0.0f, 0.1f, 0.1f, 1.0));
  auto* zwrite = dd->depthAttachment();
  zwrite->setTexture(depth_buf);
  zwrite->setLoadAction(MTL::LoadActionClear);
  zwrite->setStoreAction(MTL::StoreActionDontCare);
  zwrite->setClearDepth(0);
  MTL4::RenderCommandEncoder* renderpass_encoder = cb->renderCommandEncoder(dd);

  set_viewport(w, h, 0, 1, 0, 0);

  argument_table->setAddress(static_cast<MTL::Buffer*>(vertices)->gpuAddress(), 28);
  argument_table->setAddress(llcb->gpuAddress(), 1);

  auto rdesc = MTL::ResidencySetDescriptor::alloc()->init()->autorelease();
  auto* rs = device->newResidencySet(rdesc, &err)->autorelease();
  rs->addAllocation(static_cast<MTL::Buffer*>(vertices));
  rs->addAllocation(static_cast<MTL::Buffer*>(indices));
  rs->addAllocation(depth_buf);
  rs->addAllocation(llcb);
  rs->commit();
  cq->addResidencySet(rs);
  cq->addResidencySet(display_layer->residencySet());

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

  cq->removeResidencySet(rs);
  cq->removeResidencySet(display_layer->residencySet());
  dd->release();
  pool->release();
}

#endif
