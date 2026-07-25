#pragma once

#include "vkdefines.h"

namespace juye{

  template<uint32_t _Size>
  void vlkGetPlatformInstanceExtenstions(bcl::small_vector<const char*, _Size>* ext){
    #if __APPLE__
      if(!glfwInit()){
        juye_runtime_error();
      };
        uint32_t count;
        const char** ppExtensions = glfwGetRequiredInstanceExtensions(&count);
        for(int i = 0; i < count; ++i){
        ext->push_back(ppExtensions[i]);
        ext.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
      }

      glfwTerminate();
    #endif

    #if _WIN32
      ext->push_back("VK_KHR_win32_surface");
    #endif
  }

  VkSurfaceKHR vlkCreatePlatformSurface(VkInstance instance, void* handle);

}
