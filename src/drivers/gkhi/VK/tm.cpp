#include "vulk.h"
#include "vulkan/vulkan.h"

int gdi_device::init_driver(){
  
  VkApplicationInfo app{};
  app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app.pNext = nullptr;
  app.pApplicationName = "vk";
  app.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app.pEngineName = "vk";
  app.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app.apiVersion = VK_API_VERSION_1_0;

  //instance
  
  // VkInstanceCreateInfo instance{};
  // instance.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  // instance.pNext = nullptr;
  // instance.pApplicationInfo = &cApp;
  // instance.enabledLayerCount = instanceLayers.size();
  // instance.ppEnabledLayerNames = instanceLayers.data();
  // instance.enabledExtensionCount = extentions.size();
  // instance.ppEnabledExtensionNames = extentions.data();
  // cInstance.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  // vkcall(vkCreateInstance(&cInstance, nullptr, &instance))

  //physical device and queue familes
  
  //logical device

  //surface
  
  return 0;
}

void gdi_device::set_projection(const float* v, const float* p){

}

// gdi_memory allocate_buffer(gdi_rpool pool, size_t n_bytes, const uint64_t flags){}
//
gdi_memory gdi_device::allocate_texture(gdi_rpool pool, size_t w, size_t h, uint32_t mips){
  return nullptr;
}

gdi_rpool  gdi_device::allocate_resource_pool(){
  return nullptr;
}

gdi_transform gdi_device::generate_transform(){
  return 0;
}

void gdi_device::write_texture(void* src, gdi_memory mem, size_t width, size_t height, uint32_t mip){}
void gdi_device::write_transform(gdi_transform t, void* src){}
void gdi_device::commit_resource_pool(gdi_rpool pool){}
void gdi_device::set_constant_buffer(gdi_memory buf){}
void gdi_device::set_viewport(float w, float h, float n, float f, float x, float y){}
void gdi_device::submit_frame(){}
void gdi_device::execute_single_draw(){}

void gdi_device::dummy_draw(void* vertices, void* indices, size_t n_indices, gdi_transform transform, gdi_memory user_cbuf,
gdi_memory texture, gdi_rpool u_pool){}
