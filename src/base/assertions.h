#pragma once

#if !defined(NDEBUG)
constexpr const char* juye_base_from_path(const char* p) noexcept {
  const char* f = p;
  for (; *p; ++p) if (*p == '/' || *p == '\\') f = p + 1;
  return f;
}

#if defined(__FILE_NAME__)
  #define __JUYE_BASE_FILE __FILE_NAME__
#else
  #define __JUYE_BASE_FILE juye_base_from_path(__FILE__)
#endif


void juye_assert(const char* expr, const char* msg, const char* file, int line, bool con_msg = false);

#define _juye_assertf(_expr, _msg) if(!(_expr)){juye_assert(#_expr, _msg, __JUYE_BASE_FILE, __LINE__, true);}
#define _juye_assert(_expr) if(!(_expr)){juye_assert(#_expr, "", __JUYE_BASE_FILE, __LINE__, false);}
#else
#define _juye_assertf()
#define _juye_assert()

#endif
