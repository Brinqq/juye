#pragma once

#include <string>

namespace juye{

struct cmdline_config{
  std::string client_dll;
};

cmdline_config plt_process_arguments(int n_args, char* args[]);

}// namespace juye 
