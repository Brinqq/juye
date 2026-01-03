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
      }

      glfwTerminate();
    #endif
  }

  VkSurfaceKHR vlkCreatePlatformSurface(void* handle);

}
