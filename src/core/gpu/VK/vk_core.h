#pragma once

#include "vk_defines.h"

#include <vector>
#include <array>

#include <list>

//tmp
#include "gk/prefabs.h"
#include "glm/glm.hpp"
//---------

//TODO: MAJOR, Right now we just duplicate texture and buffers we need to cache these resources
//with xxHash and a LUT/Hashmap. 
// - add reference count to these so we know when to remove on deletion of said resource.
// - include functions for getting the cached handle for a optinal optimization for the frontend,
//   this eleminates having to rehash the gpu resource on every draw target creation.
// - figure out how to invalidate a handle if ref count hits 0 and resource is destoryed or perhaps throw a error.
// - make sure to defer deletion of ref counted objects to a state where there are no in-flight frames.
// - Thread safety?

//TODO: MAJOR, Go through a support windows and add dynamic state information based on gpu characteristics.

struct Device;
class VK;

enum FixedPasses{
  FixedPassGeometry = 0, // for now this is lighting is calculated and applied in this pass.
  FixedPassMax = 1
};

enum LightingType{
  LightingTypePoint,
  LightingTypeDirectional,
  LightingTypeAmbient
};

struct GeometryData{
  void* pVertex;
  void* pIndices;
  void* texture;

  size_t vertexBytes;
  size_t indicesBytes;
  uint32_t textureWidth;
  uint32_t textureHeight;

  uint32_t numIndices;
};

struct LightData{
  LightingType type;
  float color[4];
};


class VK{
private:

  //static pipeline

  struct Semaphore{
   static constexpr int RenderReady = 0;
   static constexpr int RenderDone = 1;
   static constexpr int CopiesDone = 2;
   static constexpr int Count = 3;
  };

  struct Fence{
    static constexpr int FrameInFlight = 0;
    static constexpr int Count = 1;
  };

  struct Sampler{
    static constexpr int ClampTexture = 0;
    static constexpr int Count = 1;

  };

  enum GpuResourceType{
      GpuResourceBuffer,
      GpuResourceImage,
  };

  enum GpuReadMemoryType{
    GpuReadMemoryLinear,
    GpuReadMemoryUnspecified,
    GpuReadMemoryTransferReady,
    GpuReadMemoryShaderReady
  };


  enum RenderPipelineFlags{
    RenderPipelineDepthEnable = 0x0,
    RenderPipelineWireframe = 0x2
  };


  struct DrawView{
    VkViewport viewport;
    VkRect2D scissor;
    float viewMatrices[12];
  };

  struct DepthBuffer{
    VkImage image;
    VkImageView view;
    VkDeviceMemory memory;
    VkFormat format;
  };


  struct GpuBuffer{
    VkBuffer handle;
    VkDeviceMemory memory;
  };

  struct GpuTexel{
    VkImage handle;
    VkDeviceMemory memory;

    //TODO: these should not be coupled with the GpuTexel because it duplicates with same textured geomtery.
    VkImageView view;
    VkDescriptorSet descriptor;
  };

  struct GeometryPassPush{
    float transform[16];
    float view[16];
    float projection[16];
  };
  
  //TODO: change from std::list::iterators when bcl::dense_table change.
  //32 bytes per iterator wtf.
  struct GBufEntry{
    std::list<GpuBuffer>::iterator vertex;  
    std::list<GpuBuffer>::iterator index;  
    std::list<GpuTexel>::iterator texture;  
    void* push;
    uint32_t numIndices;
  };


  // compile time state
  
  static constexpr uint64_t _stagingBufferSize = 80000000;
  static constexpr int _macosDeviceLocalFlag = 0;
  static constexpr int _macosHostAccessFlag = 1;

  // fixed state
    const VkComponentMapping defaultTextureCMapping{ 
      VK_COMPONENT_SWIZZLE_R,
      VK_COMPONENT_SWIZZLE_G,
      VK_COMPONENT_SWIZZLE_B,
      VK_COMPONENT_SWIZZLE_A
    };

  std::array<VkSampler, Sampler::Count> fiSamplers;

  GeometryPassPush DefaultGPassStub{};

  //dyn state
  
  //TODO: these are so so bad replace with bcl::dense_table or bcl::pool when done verifying completeness.
  std::list<GpuBuffer> bufferList;
  std::list<GpuTexel> texelList;
  std::list<GBufEntry> geometryList;
  std::vector<GBufEntry> drawList;

  public:
    typedef std::list<GBufEntry>::iterator GeoHandle;
  private:
  
  VkRenderPass renderpass[FixedPassMax];
  DepthBuffer depthBuffer;

  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice gpu;

  VkSurfaceKHR surface;
  VkSwapchainKHR swapchain;
  VkExtent2D swapchainExtent;

  uint8_t numBackbuffers = 2;
  uint32_t curBackBuffer = 0;
  VkImageView swapchainViews[2];
  VkFramebuffer framebuffers[2];
  std::array<VkSemaphore, 2> swapSemaphores;

  VkRenderPass mainRenderpass;
  VkPipeline mainPipeline;
  VkDescriptorPool geoPassDescriptorPool;
  VkDescriptorSetLayout geoPassDescriptorLayout;
  VkPipelineLayout geoPassPipelineLayout;
  std::array<VkDescriptorSet, 2> textureDescSet;

  VkCommandPool graphicsPool;
  VkCommandPool transferPool;

  VkCommandBuffer mainCommandBuffer;
  VkFence mainFence;

  VkQueue graphicQueue;
  VkQueue transferQueue;

  DrawView view;

  std::vector<QueueFamily> queueFamilies;
  std::pair<VkBuffer, VkDeviceMemory> stagingBuffers[2];
  std::array<VkSemaphore, Semaphore::Count> semaphores;
  std::array<VkFence, Fence::Count> fences;

  ivk::FeatureSet features;

  struct{
    const ssf::prefabs::TexturedCube<uint16_t> data;
    VkBuffer vbo;
    VkDeviceMemory vHandle;
    uint64_t vDataSize;

    VkBuffer ibo;
    VkDeviceMemory iHandle;
    uint64_t iDataSize;

    VkImage texture;
    VkImageView textureView;
    VkDeviceMemory texHandle;

    std::array<VkDescriptorSet, 2> textureDescSet;
    VkSampler sampler;

    glm::mat4 mvp[3];
    

  }_TmpCube;

private:
  //Dynamic state Initialization
  VkPipeline CreateRenderPipeline(const RenderPipelineFlags flags);

  //fixed State Initialization
  void CreateFixedSamplers(bool rebuild);
  void CreateFixedDescriptors();

  void SwapBackBuffers();

  void SetGpuImageBarriers(VkCommandBuffer cmdBuf, const VkImageMemoryBarrier* const pBarrier, uint32_t count, VkPipelineStageFlags src, VkPipelineStageFlags dst);

  void GpuUploadBufData(VkCommandBuffer cmdBuf, VkDeviceMemory stage, VkBuffer srcBuf, VkBuffer dstBuf, const void* const pData, size_t bytes);
  void GpuUploadImageData(VkCommandBuffer cmdBuf,VkExtent3D extent, VkDeviceMemory stage, VkBuffer srcBuf, VkImage dstBuf, const void* const pData);

  void DestroyGraphicPipeline();
  
public:

  int Init();
  void Destroy();

  int CreateComputeState();
  int DestroyComputeState();

  GeoHandle CreateGeometry(const GeometryData& geo);
  void DestroyGeometry(GeoHandle& geometry);

  void MapGeometryPassPushBuf(GeoHandle& handle, void* pData);
  void UnmapGeometryPassPushBuf(GeoHandle& handle);

  void AddToDrawList(GeoHandle& geometry);
  void RemoveToDrawList(GeoHandle& geometry);

  int CreateGraphicsState(Device& device);
  int DestroyGraphicsState();

  void SetSkyBox(const void* texture, uint32_t width, uint32_t height);

  void Draw();
  void TestTriangle();

};//class VK
