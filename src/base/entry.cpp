#include "cmdline.h"

using namespace juye;

extern int engine_entry(cmdline_config config);

using namespace juye;
int main(int argc, char* argv[]){
  cmdline_config config = process_arguments(argc, argv);
  engine_entry(config);
  return 0;
}
