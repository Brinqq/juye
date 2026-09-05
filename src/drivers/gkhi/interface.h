#pragma once
#include "types.h"

//===========================
// Struct - Device
//===========================
// create_buffer()
// create_texture()
// create_attachment()

// map_resource()
// unmap_resource()

// destroy_buffer()
// destroy_texture()
// destroy_attachment()

// create_viewport()
// create_encoder()

//===========================
// Struct - Encoder
//===========================
// set_pso()
// set_argumentlist()
// set_viewport()

// drawlist_begin()
// draw()
// draw_indirect()
// drawlist_end()
// submit()

// void write_texture()
// void write_buffer()
// void zero_buffer()
// void zero_texture()
// void readback_buffer()
// void readback_texture()
// void copy_buffer()
// void copy_texture()

// void lock_resource()
// void unlock_resource()


#define __JUYE_METAL 1

#if __JUYE_METAL
#include "metal/metal.h"
#endif

#if __JUYE_VULKAN
#endif


