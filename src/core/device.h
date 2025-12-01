#pragma once

#if _WIN32
#ifndef UNICODE
#define UNICODE
#endif
#endif

#include <unordered_map>

#if __APPLE__
#include "GLFW/glfw3.h"
extern std::unordered_map<int, std::atomic<int>*> PhysicalKeyMap;
#endif


class Device{
private:
  typedef void* PlatformWindowHandle;


public:
  int windowH;
  int windowW;
   //windowing
  PlatformWindowHandle GraphicsWindow;
   void CloseGraphicWindow();
   int CreateGraphicWindow(float width, float height, const char* name);
   bool IsGraphicWindowRunning();
   void Tick();

  //input


};


