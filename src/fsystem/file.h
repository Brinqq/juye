#pragma once
#include <stddef.h>

namespace juye{

struct fs_image_data{
  void* data;
  size_t bytes;
  int height;
  int width;
};

fs_image_data fs_load_image(const char* filepath);
void fs_unload_image(fs_image_data& image);
}
