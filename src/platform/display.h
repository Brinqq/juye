#pragma once
#include "drivers/keyboard.h"

#include <array>
#include <vector>

namespace juye{

static constexpr int kInputStreamMax = 100;

class DisplayDriver{
public:
  virtual int init() = 0;
  virtual void update() = 0;
  virtual void destroy() = 0;
  virtual bool is_running() = 0;
  virtual void* handle() = 0;
  virtual bool poll_key(KeyCode code) = 0;
  virtual ~DisplayDriver() = default;

  //NOTE: Input stream is cleared every frame, so proccessing must
  //be done prior to calling ::update().
};


#if _WIN32
#include "windows.h"
#include "glfw/glfw3.h"


class DisplayWin32 : public DisplayDriver{
private:
private:
  void ProcessMessages();
public:
  GLFWwindow* m_handle;
  bool m_running = false;

  virtual int init() override;
  virtual void update() override;
  virtual void destroy() override;
  virtual bool is_running() override;
  virtual void* handle() override;
  virtual bool poll_key(KeyCode code) override;
  ~DisplayWin32();

};//class DipslayWin32

#endif

#if __APPLE__
class DisplayOSX : public DisplayDriver{
private:
  bool m_running = false;
  GLFWwindow* m_handle;
public:

  virtual int init() override;
  virtual void update() override;
  virtual void destroy() override;
  virtual bool is_running() override;
  virtual void* handle() override;
  virtual bool poll_key(KeyCode code) override;
  ~DisplayOSX();
};

#endif

}
