#include "platform/cmdline.h"
#include "windows.h"

namespace juye{

//NOTE: char* is expected to be a wchar_t* args[]
cmdline_config plt_process_arguments(int n_args, char* args[]){
  cmdline_config ret{};

  for(int i = 0; i < n_args; i++){
    if(!wcscmp((LPWSTR)args[i], L"-proc") && i + 1 < n_args){ 
      i++;
      int n_bytes = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)args[i], -1, nullptr, 0, nullptr, nullptr);
      ret.client_dll = std::string(n_bytes, '\0');
      WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)args[i], -1, ret.client_dll.data(), n_bytes, nullptr, nullptr);
    };
  }
  return ret;
}

}
