#pragma once

#include "global.h"

void juye_crashf(const char* file, int line, const char* msg);

#define _juye_crashf(msg, ...) juye_crashf(__FILE__, __LINE__, msg);
