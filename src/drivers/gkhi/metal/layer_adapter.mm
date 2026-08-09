// The only Objective-C++ TU in the engine. Compiled with ARC (-fobjc-arc,
// set in CMake) so the __bridge casts below are valid and no manual
// retain/release is needed: the content view retains the layer.

#include "layer_adapter.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace juye {

void* metal_layer_attach(const LayerConfig& config) {
  GLFWwindow* gw = static_cast<GLFWwindow*>(config.glfw_window);
  NSWindow*   nw = glfwGetCocoaWindow(gw);

  CAMetalLayer* layer = [CAMetalLayer layer];
  layer.device               = (__bridge id<MTLDevice>)config.device;
  layer.pixelFormat          = (MTLPixelFormat)config.pixel_format;
  layer.maximumDrawableCount = config.max_drawables;
  layer.displaySyncEnabled   = config.display_sync ? YES : NO;
  layer.framebufferOnly      = YES; // GPU cannot sample/blit the drawable;
                                    // flip to NO if you ever post-process
                                    // directly from the backbuffer.

  // Retina: contentsScale in points->pixels, drawableSize in raw pixels.
  // GLFW's framebuffer size is already in pixels, so use it directly.
  int fb_w = 0, fb_h = 0;
  glfwGetFramebufferSize(gw, &fb_w, &fb_h);
  layer.contentsScale = nw.backingScaleFactor;
  layer.drawableSize  = CGSizeMake(fb_w, fb_h);

  nw.contentView.layer      = layer;
  nw.contentView.wantsLayer = YES;

  return (__bridge void*)layer;
}

void metal_layer_resize(void* raw_layer, void* glfw_window) {
  GLFWwindow*   gw    = static_cast<GLFWwindow*>(glfw_window);
  CAMetalLayer* layer = (__bridge CAMetalLayer*)raw_layer;

  int fb_w = 0, fb_h = 0;
  glfwGetFramebufferSize(gw, &fb_w, &fb_h);
  if (fb_w == 0 || fb_h == 0) return; // minimized

  NSWindow* nw = glfwGetCocoaWindow(gw);
  layer.contentsScale = nw.backingScaleFactor;
  layer.drawableSize  = CGSizeMake(fb_w, fb_h);
}

} // namespace juye
