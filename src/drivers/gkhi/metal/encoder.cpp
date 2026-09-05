#include "metal.h"

using namespace juye;

gdi_encoder gdi_device::create_encoder(){
  gdi_encoder ret{};
  return ret;
};



void gdi_encoder::set_viewport(const gdi_viewport& vp){
  MTL::Viewport viewport;
  viewport.originX = vp.xs;
  viewport.originY = vp.ys;
  viewport.width = vp.width;
  viewport.height = vp.height;
  viewport.znear = vp.near;
  viewport.zfar = vp.far;
  commands->setViewport(viewport);
}

void gdi_encoder::record(){}
void gdi_encoder::commit(){}
void gdi_encoder::reset(){}
