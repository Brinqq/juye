#include "display.h"
#include <stdio.h>

#ifdef _WIN32

#include "windows.h"
#include "windowsx.h"
#include "dwmapi.h"

using namespace juye;

static constexpr COLORREF kBorderColor = 0x00ffffff;
LPCSTR kWndClassName = "JuyeDisplayWC";
LPCSTR kApplicationName = "juye";

LRESULT CALLBACK DefaultWindowProcCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    DisplayWin32* pDisplay = nullptr;
    switch (uMsg)
      {

      case WM_ERASEBKGND:
        return 1;

      case WM_PAINT:
        return 0;

      case WM_CREATE:{
        CREATESTRUCT* info = reinterpret_cast<CREATESTRUCT*>(lParam);
        DisplayWin32* pDevice = reinterpret_cast<DisplayWin32*>(info->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(pDisplay));
        pDisplay = reinterpret_cast<DisplayWin32*>(LONG_PTR(GetWindowLongPtr(hwnd, GWLP_USERDATA)));
        return 0;
        return 0;
      }
      
      case WM_SIZE:{
        if(pDisplay){
          uint32_t width = LOWORD(lParam);
          uint32_t height = HIWORD(lParam);
          pDisplay->mWidth = width;
          pDisplay->mHeight = height;
        };
        return 0;
      }

      case WM_DESTROY: { 
        PostQuitMessage(0);
        pDisplay->Destroy();
        return 0;
      }

     }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


bool juye::DisplayWin32::IsRunning(){
  return mIsRunning;
}

int juye::DisplayWin32::Init(){
  HINSTANCE ih = GetModuleHandleA(NULL);



  WNDCLASS wc = {};
  wc.hInstance = ih;
  wc.lpszClassName = kWndClassName;
  wc.lpfnWndProc = DefaultWindowProcCallback;
  RegisterClass(&wc);

  LPCSTR k = "n";

  mHandle = CreateWindowEx(0, kWndClassName, kApplicationName, WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 
  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
  NULL, NULL, ih, this);

  if (mHandle == NULL){ 
    return 1;
  }

  COLORREF col = kBorderColor;
  DwmSetWindowAttribute(mHandle, DWMWA_BORDER_COLOR, &col, sizeof(COLORREF));

  RECT area{};
  GetWindowRect(mHandle, &area);
  mHeight = area.bottom;
  mWidth = area.right;
  
  RegisterWindowMessageA("WUM_CLIENT_DESTROY");
  ShowWindow(mHandle, 1);
  mIsRunning = true;
  return 0;
}


void juye::DisplayWin32::Update(){
  ProcessMessages();
}

void* juye::DisplayWin32::Handle(){
  assert(IsWindow(mHandle));
  return &mHandle;
}

void juye::DisplayWin32::Destroy(){
  if(mIsRunning){
    mIsRunning = false;
    DestroyWindow(mHandle);
  }
}

bool DisplayWin32::PollKey(KeyCode code){
  int x = _juye_translate_key(code);
  if(GetAsyncKeyState(x) & 0x8000){
    return true;
  };
  return false;
}

void DisplayWin32::ProcessMessages(){
  MSG msg;

  while(PeekMessageA(&msg, mHandle, 0,0, PM_REMOVE)){
   TranslateMessage(&msg);

   switch (msg.message){
     case WM_KEYDOWN: return;
     case WM_KEYUP: return;
    default:
      DispatchMessageA(&msg);
   }
  }
}

// #define WCM_DESTROY (WM_USER+0)
//
// static VirtualKeyboard g_defaultKeyboard{};
//
// LRESULT CALLBACK defaultWindowProcCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
//     Device* pDevice = reinterpret_cast<Device*>(LONG_PTR(GetWindowLongPtr(hwnd, GWLP_USERDATA)));
//     switch (uMsg)
//       {
//
//       case WM_KEYDOWN: {
//         break;
//       }
//
//       case WM_CREATE:{
//         CREATESTRUCT* info = reinterpret_cast<CREATESTRUCT*>(lParam);
//         Device* pDevice = reinterpret_cast<Device*>(info->lpCreateParams);
//         SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(pDevice));
//         pDevice = reinterpret_cast<Device*>(LONG_PTR(GetWindowLongPtr(hwnd, GWLP_USERDATA)));
//         break;
//       }
//
//       case WM_SIZE:{
//         if(pDevice){
//           uint32_t width = LOWORD(lParam);
//           uint32_t height = HIWORD(lParam);
//           pDevice->Resize(width, height);
//         };
//
//         break;
//       }
//
//       case WM_DESTROY: { 
//         PostQuitMessage(0);
//         pDevice->CloseWindow();
//         break;
//       }
//
//     }
//
//     return DefWindowProc(hwnd, uMsg, wParam, lParam);
// }
//
// void Device::CloseWindow(){
//   _Window.runningFlag = false;
//   DestroyWindow(_Window.handle);
// }
//
// void Device::ProcessWindowInput(){
//   cassert(_Window.handle != NULL);
//   MSG msg = { };
//   while (PeekMessage(&msg, _Window.handle, 0, 0, PM_REMOVE) != 0){
//     TranslateMessage(&msg);
//
//     switch (msg.message)
//       {
//
//         case WM_MOUSEMOVE:{
//           // uint32_t x = pKeyboard->mouse.x;
//           // uint32_t y = pKeyboard->mouse.y;
//           // pKeyboard->mouse.x = GET_X_LPARAM(msg.lParam);
//           // pKeyboard->mouse.y = GET_Y_LPARAM(msg.lParam);
//           // pKeyboard->mouse.dstY = y - pKeyboard->mouse.y; 
//           // pKeyboard->mouse.dstX = x - pKeyboard->mouse.x; 
//           break;
//         };
//
//         
//
//         case WM_KEYDOWN:{
//           pKeyboard->keys[msg.wParam] = 1;
//           break;
//         }
//
//         case WM_KEYUP:{
//           pKeyboard->keys[msg.wParam] = 0;
//           break;
//         }
//
//
//         default:{
//           DispatchMessage(&msg);
//         }
//
//       }
//
//
//     };
//
// }
//
//
//
// int Device::CreateInternalWindow(HINSTANCE instanceHandle, const wchar_t* className){
//   WNDCLASS wc = {};
//   wc.hInstance = instanceHandle;
//   wc.lpszClassName = className;
//   wc.lpfnWndProc = defaultWindowProcCallback;
//   RegisterClass(&wc);
//   _Window.handle = CreateWindowEx(0, className, L"Game", WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 
//   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
//   NULL, NULL, instanceHandle, this);
//
//   if (_Window.handle == NULL){ 
//     LOG_ERROR("Failure to create Win32 window.");
//     return 0;
//   }
//
//   _Window.runningFlag = true;
//
//   ShowWindow(_Window.handle, 1);
//
//   const COLORREF borderColor = 0x00ffffff;
//   DwmSetWindowAttribute(_Window.handle, DWMWA_BORDER_COLOR, &borderColor, sizeof(COLORREF));
//   RegisterWindowMessageA("WUM_CLIENT_DESTROY");
//
//   pKeyboard = &g_defaultKeyboard;
//
//   RECT area;
//   GetWindowRect(_Window.handle, &area);
//   _Window.height = area.bottom;
//   _Window.width = area.right;
//   
//   CreateDriverContext();
//   return 0;
// }
//
// int Device::CreateDriverContext(){
//   GpuDriverConfig config{};
//   config.win = _Window.handle;
//   config.debug = GpuDriverDebugLevel::All;
//   driver = new DX12();
//   driver->Init(config);
//   _Window.driverFlag = true;
//   return 0;
// }
//
// void Device::Resize(const uint32_t width, const uint32_t height){
//   log_trace("Window Resize event - Handle id: %i - %ix%i", _Window.handle, width, height);
//   _Window.width = width;
//   _Window.height = height;
//   if(_Window.driverFlag == true){
//     driver->HandleWindowResize(_Window.width, _Window.height);
//   }
// }
//
// void Device::Update(){
//   ProcessWindowInput();
// }
//
// int Device::SetVirtualKeyboard(VirtualKeyboard& keyboard){
//   this->pKeyboard = &keyboard;
//   return 0;
// }
//
// void Device::GetWindowSize(uint32_t* width, uint32_t* height){
//   *width = _Window.width; 
//   *height = _Window.height;
// }
//
// bool Device::IsWindowValid(){return _Window.runningFlag;}
//
#endif
