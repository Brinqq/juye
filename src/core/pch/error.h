#pragma once
#include "core/global.h"


inline void _juye_runtime_error(const char* file, int line){
  printf("ssf has expeirenced a runtime error. thrown from %s:%i\n", file, line);
  std::abort();
};

#define juye_runtime_error() _juye_runtime_error(__FILE__, __LINE__);
#define juye_runtime_errorf() _juye_runtime_error(__FILE__, __LINE__);


#if _RELEASE
#define juye_crashf(msg, ...)
#else
#define juye_crashf(msg, ...)
#endif
