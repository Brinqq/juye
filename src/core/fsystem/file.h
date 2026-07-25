#pragma once

namespace juye{

struct ImageData{
  void* data;
  size_t bytes;
  int height;
  int width;
};

ImageData LoadI(const char* filepath);
void UnloadImage(ImageData& image);
}
