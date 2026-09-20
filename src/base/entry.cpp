#include "platform/cmdline.h"

using namespace juye;

extern int engine_entry(cmdline_config config);

using namespace juye;


#if _WIN32
#include "windows.h"
int WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmd, int n_cmds){
  LPWSTR cmds = GetCommandLineW();
  int n_args = 0;
  LPWSTR* args = CommandLineToArgvW(cmds, &n_args);

  //note this is recasted back to wchar in function.
  cmdline_config conf = plt_process_arguments(n_args, (char**)args);

  LocalFree(args);
  engine_entry(conf);
  return 0;
}

#else
int main(int argc, char* argv[]){
  cmdline_config config = plt_process_arguments(argc, argv);
  engine_entry(config);
  return 0;
}
#endif
