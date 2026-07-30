#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

namespace juye{

void mtl_append_vertex_attribute( const MTL::VertexDescriptor& desc,
                              int object,
                              int offset, 
                              int index,
                              MTL::VertexFormat format);

}
