#pragma once
#include "keyboard.h"

#include <array>

namespace juye{

struct DisplayInputEntry{
  enum KeyCode code;
};

class DisplayDriver{
public:
  virtual int Init() = 0;
  virtual void Update() = 0;
  virtual void Destroy() = 0;
  virtual bool IsRunning() = 0;
  virtual void* Handle() = 0;

  //NOTE: Input stream is cleared every frame, so proccessing must
  //be dont prior to calling ::update().
  virtual DisplayInputEntry* MapInputStream() = 0;
};

}//namespace juye

#if _WIN32
#include "windows.h"

namespace juye{

class DisplayWin32 : public DisplayDriver{
private:
static constexpr int kInputStreamMax = 100;
  HWND mHandle;
  int mWidth, mHeight;
  bool mIsRunning = false;
public:
  virtual int Init() override;
  virtual void Update() override;
  virtual void Destroy() override;
  virtual bool IsRunning() override;
  virtual void* Handle() override;
  virtual DisplayInputEntry* MapInputStream() override;

};//class DipslayWin32
}//namespace juye

#endif


#if __APPLE__
class DisplayOSX : DisplayDriver{

};

#endif

