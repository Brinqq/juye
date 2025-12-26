#pragma once

#include <cassert>
#include <stdio.h>
#include <cstdlib>

#if !defined(KiB)
#define KiB (1ULL << 10)
#endif

#if !defined(MiB)
#define MiB (1ULL << 20)
#endif

#if !defined(GiB)
#define GiB (1ULL << 30)
#endif
