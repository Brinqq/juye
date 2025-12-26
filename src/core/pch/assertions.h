#pragma once

#if !defined(NDEBUG)

inline void _juye_assert_msg(const char* expr, const char* msg, const char* file, int line){
};


#define juye_assertf(_expr, _msg) if(!_expr){_juye_assert_msg(#_expr, #_msg, __FILE__, __LINE__)}
#else
#define juye_assertf()
#endif
