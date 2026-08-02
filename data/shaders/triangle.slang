
struct VertexOut{
  float4 pos : SV_Position;
  float3 color : COLOR0;
};

struct VertexIn{
  float3 pos : POSITION;
  float2 uv : TEXCOORD0;
  float3 normal : NORMAL;
};


struct llcb_data{
  float4x4 view;
  float4x4 projection;
};

struct transform_data{
  float4x4 m;
};

struct geometry_cb{
  uint transform_offset;
};

ConstantBuffer<llcb_data> llcb;
StructuredBuffer<transform_data> transform;
ConstantBuffer<geometry_cb> user_dat;


[shader("vertex")]
VertexOut vertex_main(VertexIn in){
    VertexOut ret;
    float4x4 vp = mul(llcb.projection, llcb.view);
    float4 w = mul(transform[user_dat.transform_offset].m, float4(in.pos, 1.0f));
    float4 final_pos = mul(vp, w);
    ret.pos = final_pos;
    ret.color = float3(0.0, 1.0, 0.0);
    return ret;
}


[shader("pixel")]
float4 pixel_main(VertexOut in) : SV_Target{
  return float4(in.color, 1);
}
