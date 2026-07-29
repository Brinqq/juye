
struct VertexOut{
  float4 pos : SV_Position;
  float3 color : COLOR0;
};

struct VertexIn{
  float3 pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
};

StructuredBuffer<VertexIn> vertices;


[shader("vertex")]
VertexOut vertex_main(VertexIn in){
    VertexOut ret;
    ret.pos = float4(in.pos, 1.0f);
    ret.color = float3(1.0f, 0.0f, 0.0f);
    return ret;
}

[shader("pixel")]
float4 pixel_main(VertexOut in) : SV_Target{
  return float4(in.color, 1);
}
