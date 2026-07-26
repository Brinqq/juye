#include "Foundation/NSTypes.hpp"
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "metal_core.h"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

// #include "layer_adapter.h"
#include "core/pch/assertions.h"
#include "core/pch/error.h"


//triangle test
// All temporary
//

#define MAX_FRAMES_IN_FLIGHT

namespace{

static const float vertices[]{
  -0.5, -0.5,
  0.0, 0.5,
  0.5, 0.5
};

static const uint8_t indices[]{
  0,1,2
};


MTL::Device* device;
MTL4::CommandQueue* cq;
MTL4::CommandBuffer* cb;
MTL4::CommandAllocator* ca[2];
MTL::Library* library;
MTL::SharedEvent* buffer_sync;
MTL::ResidencySet* residency_set;
MTL4::Compiler* compiler;
CA::MetalLayer* mtl_layer;
MTL::SharedEvent* frame_sync;
MTL4::ArgumentTable* argument_table;

MTL::Buffer* vertex_buf[2];

uint64_t cur_frame = 0;

void create_renderer(){
  NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
  NS::Error* err = NS::Error::alloc()->init()->autorelease();

  // Basic Handles
  device = MTL::CreateSystemDefaultDevice();

  //require mtl4
  if(!device->supportsFamily(MTL::GPUFamilyMetal4)){
    _juye_crashf("Requires metal 4, update OSX version");
  }

  cq = device->newMTL4CommandQueue();
  cb = device->newCommandBuffer();
  library = device->newDefaultLibrary();

  // - command allocator
ca[0] = device->newCommandAllocator();
  ca[1] = device->newCommandAllocator();

  // -- Event
  //
  frame_sync = device->newSharedEvent();
  buffer_sync = device->newSharedEvent();
  cur_frame = buffer_sync->signaledValue();

  // -- Buffers
  vertex_buf[0] = device->newBuffer(sizeof(vertices), MTL::ResourceStorageModeShared);
  vertex_buf[1] = device->newBuffer(sizeof(vertices), MTL::ResourceStorageModeShared);

  // -- Viewport

  // -- Argument Table
  MTL4::ArgumentTableDescriptor* argument_desc = MTL4::ArgumentTableDescriptor::alloc()->init()->autorelease();
  argument_desc->setMaxBufferBindCount(2);
  argument_table = device->newArgumentTable(argument_desc, &err);

  
  // -- Residency Set
  //
  auto rdesc = MTL::ResidencySetDescriptor::alloc()->init()->autorelease();
  residency_set =  device->newResidencySet(rdesc, &err);
  residency_set->addAllocation(vertex_buf[0]);
  residency_set->addAllocation(vertex_buf[1]);
  residency_set->commit();

  cq->addResidencySet(residency_set);

  mtl_layer = CA::MetalLayer::layer();
  cq->addResidencySet(mtl_layer->residencySet());


  // -- Pipeline state

  // - Render pipeline
  //
  auto* pipeline_desc = MTL4::RenderPipelineDescriptor::alloc()->init()->autorelease();

  // - Color Attachment
  pipeline_desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatRGBA8Snorm); 

  // -  vertex and pixel function descriptors
  //
  MTL4::LibraryFunctionDescriptor* vs_function_desc = MTL4::LibraryFunctionDescriptor::alloc()->init()->autorelease();
  vs_function_desc->setLibrary(library);
  vs_function_desc->setName(MTLSTR("vertexShader"));
  pipeline_desc->setVertexFunctionDescriptor(vs_function_desc);
  MTL4::LibraryFunctionDescriptor* ps_function_desc = MTL4::LibraryFunctionDescriptor::alloc()->init()->autorelease();
  ps_function_desc->setLibrary(library);
  ps_function_desc->setName(MTLSTR("pixelShader"));
  pipeline_desc->setFragmentFunctionDescriptor(ps_function_desc);
  
  // - compiler
  auto* desc = MTL4::CompilerDescriptor::alloc()->init()->autorelease();
  compiler = device->newCompiler(desc, &err);
  compiler->newRenderPipelineState(pipeline_desc, nullptr, &err);

  pool->release();
}

}// Namespace anon

void draw_frame(){

  // this is fine since we stall at the end of frame for now.
  auto& command_allocator =  ca[0];
  command_allocator->reset(); 
  cb->beginCommandBuffer(command_allocator);

  auto* dd = MTL4::RenderPassDescriptor::alloc()->init();
  MTL4::RenderCommandEncoder* renderpass_encoder = cb->renderCommandEncoder(dd);
  dd->release();

  MTL::Viewport vp;
  vp.height = 1080;
  vp.width = 1920;
  vp.originX = 0;
  vp.originY = 0;
  vp.zfar = 1.0;
  vp.znear = 0.0;

  argument_table->setAddress(vertex_buf[0]->gpuAddress(), 0);

  renderpass_encoder->setViewport(vp);
  renderpass_encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, 0, 3);
  renderpass_encoder->endEncoding();
  cb->endCommandBuffer();
  cq->commit(&cb, 1);

  uint64_t f = 0;
  cq->signalEvent(frame_sync, ++f);
  frame_sync->waitUntilSignaledValue(f, 1000);
}

void metal_test(){
  create_renderer();
}

void destroy(){
  
}

