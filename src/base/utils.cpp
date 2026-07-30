#include "global.h"

#if !defined(NDEBUG)
void juye_assert(const char* expr, const char* msg, const char* file, int line, bool con_msg){
  if(con_msg){
    printf("Assertion failed: (%s), file: %s:%i, %s\n",expr, file, line, msg);
  }else{
    printf("Assertion failed: (%s), file: %s:%i\n", expr, file, line);
  }

  std::abort();
}

#endif

void juye_crashf(const char* file, int line, const char* msg){
  printf("juye crash: %s. thrown from %s:%i\n", msg, file, line);
  std::abort();
};
