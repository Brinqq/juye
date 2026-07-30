#pragma once

#if __APPLE__
#include "GLFW/glfw3.h"

const int platform_key_map[] = {
  GLFW_KEY_W,
  GLFW_KEY_A,
  GLFW_KEY_S,
  GLFW_KEY_D,
  GLFW_MOUSE_BUTTON_1,
  GLFW_MOUSE_BUTTON_2,
  GLFW_KEY_LEFT_SHIFT,
  GLFW_KEY_SPACE,
  GLFW_KEY_ESCAPE
};
#endif

#if _WIN32
#include "windows.h"
#include "winuser.h"

const int key_map[] = {
  0x57,
  0x41,
  0x53,
  0x44,
  VK_LBUTTON,
  VK_RBUTTON,
  VK_SHIFT,
  VK_SPACE,
  VK_ESCAPE
};

#endif

#define _juye_translate_key(_key) key_map[_key]

namespace juye{
  enum KeyCode{
    KeyCodeNone = -1,
    KeyCodeW = 0,
    KeyCodeA = 1,
    KeyCodeS = 2,
    KeyCodeD = 3,
    KeyCodeLMB = 4,
    KeyCodeRMB = 5,
    KeyCodeSHIFT = 6,
    KeyCodeSPACE = 7,
    KeyCodeESC = 8
  };
};
