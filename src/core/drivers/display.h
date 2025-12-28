#pragma once


class DisplayDriver{
  virtual int Init() = 0;
  virtual void Update() = 0;
  virtual void Destroy() = 0;
};


#if _WIN32
#include "windows.h"

class DisplayWin32 : DisplayDriver{
private:
  HWND mHandle;
  int mWidth, mHeight;
  bool mIsRunning = false;
public:
  virtual int Init() override;
  virtual void Update() override;
  virtual void Destroy() override;
};

#endif


#if __APPLE__
class DisplayOSX : DisplayDriver{

};

#endif
