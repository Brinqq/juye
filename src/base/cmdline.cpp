#include "cmdline.h"
#include <stdio.h>
#include <string.h>

const char* main_proc_cmp = "-proc";

namespace juye{


cmdline_config process_arguments(int n_args, char* args[]){
  cmdline_config ret{};

  for(int i = 0; i < n_args; i++){
    if(!strcmp(args[i], main_proc_cmp) && i + 1 < n_args){ 
      i++;
      ret.client_dll = args[i];
    };
  }

  return ret;
}

}
