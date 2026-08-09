#pragma once
#include <string>

namespace juye{

struct cmdline_config{
  std::string client_dll;
};

cmdline_config process_arguments(int n_args, char* args[]);

}// namespace juye 
