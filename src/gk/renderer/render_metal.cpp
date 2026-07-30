#define __JUYE_METAL 1

#if __JUYE_METAL

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "drivers/metal/metal.h"
#include "drivers/metal/mtl_helpers.h"
#include "drivers/metal/layer_adapter.h"
#include "drivers/metal//futils.h"

#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

#include "base/global.h"

#define MAX_FRAMES_IN_FLIGHT

static const float vertices[]{
  -0.5, 0.0, 0.0, 0.0, 1.0, 0.0,
  0.0, 0.5, 0.0, 0.0, 1.0, 1.0,
  0.5, 0.0, 0.0, 1.0, 0.0, 0.0
};

static const uint8_t indices[]{
  0,1,2
};

struct CBuf{
  float r;
  float g;
  float b;
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
MTL::RenderPipelineState* pso;

MTL::Buffer* vertex_buf[2];
MTL::Buffer* idx_buf;
MTL::Buffer* constant_buf;
MTL::Texture* depth_buf;
MTL::DepthStencilState* depth_state;


uint64_t cur_frame = 0;

static Scene scene{};
static std::vector<RenderEntry> render_entries;

RenderEntry create_render_entry(){
  return RenderEntry{};
}
void create_renderer(void* win){

  NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
  NS::Error* err = nullptr;

  // Basic Handles
  device = MTL::CreateSystemDefaultDevice();

  //require mtl4
  if(!device->supportsFamily(MTL::GPUFamilyMetal4)){
    _juye_crashf("Requires metal 4, update OSX version");
  }

  juye::LayerConfig cfg{win, device, MTL::PixelFormatBGRA8Unorm, 2, false};
  mtl_layer = (CA::MetalLayer*)juye::metal_layer_attach(cfg);


  // - command allocator, buffer, queue
  cq = device->newMTL4CommandQueue();
  cb = device->newCommandBuffer();
  ca[0] = device->newCommandAllocator();

  // -- Event
  frame_sync = device->newSharedEvent();
  buffer_sync = device->newSharedEvent();
  cur_frame = buffer_sync->signaledValue();

  // -- Buffers
  size_t bytes = scene.cube_red.mesh.stride * scene.cube_red.mesh.numVertices; 
  vertex_buf[0] = device->newBuffer(bytes, MTL::ResourceStorageModeShared);
  memcpy(vertex_buf[0]->contents(), scene.cube_red.mesh.pVertices, bytes);

  idx_buf = device->newBuffer(sizeof(uint16_t) * scene.cube_red.mesh.numIndices, MTL::ResourceStorageModeShared);
  memcpy(idx_buf->contents(), scene.cube_red.mesh.pIndices, sizeof(uint16_t) * scene.cube_red.mesh.numIndices);

  constant_buf = device->newBuffer(16, MTL::ResourceStorageModeShared);
  new(constant_buf->contents()) CBuf{0.0f, 0.0f, 1.0f};

  MTL::Texture* back_buffer = mtl_layer->nextDrawable()->texture();
  depth_buf = generate_depth_buffer(device, back_buffer->width(), back_buffer->height(), MTL::PixelFormatDepth32Float);

  MTL::DepthStencilDescriptor* z_desc = MTL::DepthStencilDescriptor::alloc()->init()->autorelease();
  z_desc->setDepthCompareFunction(MTL::CompareFunctionGreater);
  z_desc->setDepthWriteEnabled(1);
  depth_state = device->newDepthStencilState(z_desc);

  // -- Argument Table
  MTL4::ArgumentTableDescriptor* argument_desc = MTL4::ArgumentTableDescriptor::alloc()->init()->autorelease();
  argument_desc->setMaxBufferBindCount(32);
  argument_table = device->newArgumentTable(argument_desc, &err);
  
  // -- Residency Set
  auto rdesc = MTL::ResidencySetDescriptor::alloc()->init()->autorelease();
  residency_set =  device->newResidencySet(rdesc, &err);
  residency_set->addAllocation(vertex_buf[0]);
  residency_set->addAllocation(idx_buf);
  residency_set->addAllocation(constant_buf);
  residency_set->addAllocation(depth_buf);
  residency_set->commit();
  cq->addResidencySet(residency_set);
  cq->addResidencySet(mtl_layer->residencySet());

// -- Pipeline state
  auto* pipeline_desc = MTL4::RenderPipelineDescriptor::alloc()->init()->autorelease();


  auto* vertex_desc = MTL::VertexDescriptor::alloc()->init()->autorelease();
  juye::mtl_append_vertex_attribute(*vertex_desc, 0, 0, 28, MTL::VertexFormatFloat3);
  juye::mtl_append_vertex_attribute(*vertex_desc, 1, sizeof(float) * 3, 28, MTL::VertexFormatFloat3);
  juye::mtl_append_vertex_attribute(*vertex_desc, 2, sizeof(float) * 5, 28, MTL::VertexFormatFloat3);
  vertex_desc->layouts()->object(28)->setStride(scene.cube_red.mesh.stride);
  vertex_desc->layouts()->object(28)->setStepFunction(MTL::VertexStepFunctionPerVertex);
  pipeline_desc->setVertexDescriptor(vertex_desc);

  pipeline_desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatRGBA8Unorm); 

  // - Shader
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
}


uint64_t f = 0;
void draw_frame(){
  NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

  

  // this is fine since we stall at the end of frame for now.
  auto& command_allocator =  ca[0];
  command_allocator->reset(); 
  cb->beginCommandBuffer(command_allocator);

  auto* dd = MTL4::RenderPassDescriptor::alloc()->init();
  auto* drawable = mtl_layer->nextDrawable();

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
  renderpass_encoder->setRenderPipelineState(pso);

  // renderpass_encoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
  argument_table->setAddress(vertex_buf[0]->gpuAddress(), 28);
  argument_table->setAddress(constant_buf->gpuAddress(), 0);
  renderpass_encoder->setArgumentTable(argument_table, MTL::RenderStageVertex | MTL::RenderStageFragment);
  renderpass_encoder->setDepthStencilState(depth_state);

  scene.viewport.width = drawable->texture()->width();
  scene.viewport.height = drawable->texture()->height();

  renderpass_encoder->setViewport(scene.viewport);

  renderpass_encoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, scene.cube_red.mesh.numIndices, MTL::IndexTypeUInt16,
  idx_buf->gpuAddress(), 2 * scene.cube_red.mesh.numIndices  
  );

  cq->wait(drawable);
  renderpass_encoder->endEncoding();
  cb->endCommandBuffer();
  cq->commit(&cb, 1);
  cq->signalDrawable(drawable);
  drawable->present();
  cq->signalEvent(frame_sync, ++f);
  frame_sync->waitUntilSignaledValue(f, 1000);

  dd->release();
  pool->release();
}

void metal_test(void* win){
  create_renderer(win);
}

void destroy(){
  
}

#endif
