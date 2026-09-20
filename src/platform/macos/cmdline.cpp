#include "platform/cmdline.h"

using namespace juye;


const char* main_proc_cmp = "-proc";

cmdline_config plt_process_arguments(int n_args, char* args[]){
  cmdline_config ret{};

  for(int i = 0; i < n_args; i++){
    if(!strcmp(args[i], main_proc_cmp) && i + 1 < n_args){ 
      i++;
      ret.client_dll = args[i];
    };
  }

  return ret;
  
}
