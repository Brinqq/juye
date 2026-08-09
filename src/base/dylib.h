#pragma once


#if __APPLE__
#include <dlfcn.h>

#define JUYE_DLL_LOAD_SYMBOL(lh, sym) dlsym(lh, sym)
#define JUYE_DLL_LOAD(lib) dlopen(lib, RTLD_NOW);
#define JUYE_DLL_UNLOAD(handle) dlclose(handle);
#define JUYE_DLL_EXPORT __attribute__((visibility("default")))
#define JUYE_DLL_CHECK_HANDLE(h) !(h == nullptr)

#endif

#if _WIN32
#endif
