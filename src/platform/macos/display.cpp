#include "drivers/display.h"
#include "base/global.h"
#include "drivers/keyboard.h"
#include "GLFW/glfw3.h"

namespace juye{


void glfw_keyboard_callback(GLFWwindow* h, int key, int scan, int action, int mods){
}


int DisplayOSX::init(){
  int win_w = 1920;
  int win_h = 1080;

  if(glfwInit() == GLFW_FALSE){
    return 1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  m_handle = glfwCreateWindow(win_w, win_h, "this", nullptr, nullptr);

  if(m_handle == nullptr){
    return 1;
  }

  glfwGetWindowSize(m_handle, &win_w, &win_h);
  glfwSetKeyCallback(m_handle, &glfw_keyboard_callback);

  return 0;
}

void DisplayOSX::update(){
  if (glfwWindowShouldClose(m_handle)){
    m_running = true;
  };

  glfwPollEvents();
}

void DisplayOSX::destroy(){
  if(!m_handle){return;}

  glfwDestroyWindow(m_handle);
  glfwTerminate();
}

bool DisplayOSX::is_running(){
  return !m_running;
}

void* DisplayOSX::handle(){
  return static_cast<void*>(m_handle);
};

bool DisplayOSX::poll_key(KeyCode code){return false;};

DisplayOSX::~DisplayOSX(){
  destroy();
}

}//namespace juye
