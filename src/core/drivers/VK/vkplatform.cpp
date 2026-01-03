#include "vkplatform.h"


#if _WIN32
#include "windows.h"
#include <vulkan/vulkan_win32.h>

VkSurfaceKHR juye::vlkCreatePlatformSurface(VkInstance instance, void* handle){
  VkSurfaceKHR ret;
  VkWin32SurfaceCreateInfoKHR cSurface{};
  HWND h = *static_cast<HWND*>(handle);
  cSurface.pNext = nullptr;
  cSurface.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  cSurface.flags = 0;
  cSurface.hinstance = (HINSTANCE)GetWindowLongPtr(h, GWLP_HINSTANCE);;
  cSurface.hwnd = h ;
  vkcall(vkCreateWin32SurfaceKHR(instance, &cSurface, nullptr, &ret))
  return ret;
}

#endif

#if __APPLE__ 
VkSurfaceKHR juye::vlkGetPlatformSurface(void* handle){
    VkSurfaceKHR ret;
    vkcall(glfwCreateWindowSurface(instance, handle, nullptr, &ret));
    return ret;
}
#endif
