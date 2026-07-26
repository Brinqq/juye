#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

void* attach_metal_layer(void* nswindow, void* device) {
    NSWindow* w = (__bridge NSWindow*)nswindow;
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = (__bridge id<MTLDevice>)device;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    w.contentView.layer = layer;
    w.contentView.wantsLayer = YES;
    return (__bridge void*)layer;
}
