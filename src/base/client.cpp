#include "dylib.h"
#include "global.h"

#include <stdio.h>

static void* client_handle;

namespace juye{

void client_load_dyn(const char* dylib){
  printf("client load requested: %s\n", dylib);

  client_handle = JUYE_DLL_LOAD(dylib) 
  
  if(!client_handle){
    _juye_crashf("failed to load client dll, %s\n", dylib);
  }

 void* (*client_main)() = (void* (*)())JUYE_DLL_LOAD_SYMBOL(client_handle, "client_main");

 if(!client_main){
  _juye_crashf("failed to load client_main")
 }

  client_main();
}

void client_init(){}
void client_tick(){}
void client_update(){}
void client_shutdown(){}

}
