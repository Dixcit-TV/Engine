struct VertexIn
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 Position : SV_Position;
    float3 WorldNormal : NORMAL;
    float4 Color : COLOR;
};