#include "vkplatform.h"


#if _WIN32
VkSurfaceKHR juye::vlkCreatePlatformSurface(void* handle){
  
  return VK_NULL_HANDLE;
}
#endif

#if __APPLE__ 
VkSurfaceKHR juye::vlkGetPlatformSurface(void* handle){
    VkSurfaceKHR ret;
    vkcall(glfwCreateWindowSurface(instance, handle, nullptr, &ret));
    return ret;
}
#endif
