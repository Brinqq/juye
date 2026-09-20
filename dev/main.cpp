#include <stdio.h>

#if _WIN32
#define _export _declspec(dllexport)
#endif

#if __APPLE__
#define _export __attribute__((visibility("default")))
#endif


extern "C"{

_export void client_main(){
  printf("Hello, from client main\n");
}

}
void client_update(){
  printf("Hello, from client update");
}

void client_shutdown(){
  printf("Hello, from client shutdown");
}

