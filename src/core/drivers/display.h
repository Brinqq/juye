#pragma once
#include "keyboard.h"

#include <array>
#include <vector>

namespace juye{

static constexpr int kInputStreamMax = 100;

class DisplayDriver{
public:
  virtual int Init() = 0;
  virtual void Update() = 0;
  virtual void Destroy() = 0;
  virtual bool IsRunning() = 0;
  virtual void* Handle() = 0;
  virtual bool PollKey(KeyCode code) = 0;
  virtual ~DisplayDriver() = default;

  //NOTE: Input stream is cleared every frame, so proccessing must
  //be done prior to calling ::update().
};

}//namespace juye

#if _WIN32
#include "windows.h"

namespace juye{

class DisplayWin32 : public DisplayDriver{
private:
private:
  void ProcessMessages();
public:
  HWND mHandle;
  int mWidth, mHeight;
  bool mIsRunning = false;

  virtual int Init() override;
  virtual void Update() override;
  virtual void Destroy() override;
  virtual bool IsRunning() override;
  virtual void* Handle() override;
  virtual bool PollKey(KeyCode code) override;

};//class DipslayWin32
}//namespace juye

#endif


#if __APPLE__
class DisplayOSX : DisplayDriver{

};

#endif

