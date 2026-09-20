#include "vulk.h"

gdi_memory gdi_device::allocate_memory(const gdi_allocation_desc& memory){
  return nullptr;
}

void gdi_device::deallocate_memory(gdi_memory mem){

}

gdi_resource gdi_device::create_buffer(gdi_memory mem, const gdi_buffer_desc& d_buf, size_t offset){
  return nullptr;
}

void gdi_device::destory_buffer(gdi_resource buffer){

}

gdi_resource gdi_device::create_texture(gdi_memory mem, const gdi_texture_desc& d_tex, size_t offset){
  return nullptr;
}

void gdi_device::destory_texture(){

}

gdi_resource gdi_device::create_attachment(gdi_memory mem, const gdi_attachment_desc& d_tex, size_t offset){
  return nullptr;
}

void gdi_device::destory_attachment(){

}

void* gdi_device::map_resource(gdi_resource res, gdi_resource_type type){
  return nullptr;
}

void gdi_device::unmap_resource(gdi_resource res, gdi_resource_type type){
}




