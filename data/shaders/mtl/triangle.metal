#include <metal_stdlib>
using namespace metal;

struct vertex_main_in{
  float3 pos [[attribute(0)]];
  float3 normal [[attribute(1)]];
  float2 uv [[attribute(2)]];
};

struct vertex_main_out{
  float4 pos [[position]];
  float2 uv;
};

struct llcb_h{
    float4x4 view;
    float4x4 projection;
};

struct constant_buffer_h{
  int32_t offset;
};


[[vertex]] vertex_main_out
vertex_main(vertex_main_in in [[stage_in]],
            constant llcb_h* llcb [[buffer(0)]],
            device const float4x4* transforms [[buffer(1)]],
            constant constant_buffer_h* user_in [[buffer(2)]]){

      float4x4 model = transforms[user_in->offset];
      float4 world_pos = model * float4(in.pos, 1.0f);
      float4x4 vp =  llcb->projection * llcb->view; 
      float4 final_pos = vp * world_pos;
      return vertex_main_out{final_pos, in.uv};
}


struct pixel_main_out{
  float4 output [[color(0)]];
};


[[fragment]] pixel_main_out
pixel_main(vertex_main_out in [[stage_in]],
           texture2d<float> albedo [[texture(0)]],
           sampler sm [[sampler(0)]]){

  return pixel_main_out{albedo.sample(sm, in.uv)};
}
