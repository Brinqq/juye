#include "file.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace juye{

fs_image_data fs_load_image(const char* filepath){
  int w, h, c;
  stbi_uc* pixels = stbi_load(filepath, &w, &h, &c, STBI_rgb_alpha);
  size_t bytes = w * h * 4;
  assert(pixels != 0);
  return fs_image_data{(void*)pixels, bytes, w, h};
}


void fs_unload_image(fs_image_data& image){
  stbi_image_free(image.data);
}

}
