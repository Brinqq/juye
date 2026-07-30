
struct VertexOut{
  float4 pos : SV_Position;
  float3 color : COLOR0;
};

struct VertexIn{
  float3 pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
};


struct CBuf{
  float3 color;
};


ConstantBuffer<CBuf> cb;
StructuredBuffer<VertexIn> vertices;


[shader("vertex")]
VertexOut vertex_main(VertexIn in){
    VertexOut ret;
    ret.pos = float4(in.pos, 1.0f);
    ret.color = cb.color;
    return ret;
}

[shader("pixel")]
float4 pixel_main(VertexOut in) : SV_Target{
  return float4(in.color, 1);
}
