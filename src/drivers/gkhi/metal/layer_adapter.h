#pragma once

// Objective-C++ bridge between GLFW and CAMetalLayer.
//
// layer_adapter.mm is the ONLY translation unit in the engine that speaks
// Objective-C. Everything crosses this boundary as void*:
//   - glfw_window : GLFWwindow*
//   - device      : MTL::Device*  (same object as id<MTLDevice>)
//   - return value: CA::MetalLayer* (cast it on the C++ side)

#include <cstdint>

namespace juye {

struct LayerConfig {
  void*    glfw_window  = nullptr;
  void*    device       = nullptr;
  uint32_t pixel_format = 80;    // MTLPixelFormat raw value. 80 == BGRA8Unorm.
                                 // Pass MTL::PixelFormat... cast to uint32_t.
                                 // Valid drawable formats only: BGRA8Unorm(80),
                                 // BGRA8Unorm_sRGB(81), RGBA16Float(115),
                                 // RGB10A2Unorm(90).
  uint32_t max_drawables = 3;    // 2 or 3
  bool     display_sync  = true; // vsync
};

// Creates a CAMetalLayer, configures it, and installs it as the backing
// layer of the window's content view. Sizes the drawable to the current
// framebuffer size (pixels, retina-correct). The view owns the layer;
// its lifetime is the window's lifetime.
void* metal_layer_attach(const LayerConfig& config);

// Re-syncs drawableSize and contentsScale with the window's current
// framebuffer size. Call from the GLFW framebuffer-size callback (fires
// on resize AND on moving between monitors of different DPI).
void metal_layer_resize(void* layer, void* glfw_window);

} // namespace juye
