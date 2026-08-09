#include "mtl_helpers.h"

namespace juye{

void mtl_append_vertex_attribute(const MTL::VertexDescriptor& desc, int object, int offset, int index,
                             MTL::VertexFormat format){

  desc.attributes()->object(object)->setOffset(offset);
  desc.attributes()->object(object)->setBufferIndex(index);
  desc.attributes()->object(object)->setFormat(format);
}



}//namespace juye
