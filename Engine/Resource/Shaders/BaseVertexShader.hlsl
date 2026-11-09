#include "BaseShadertypes.hlsl"

cbuffer cbPerObject : register(b0)
{
    row_major float4x4 gWorldViewProj;
    row_major float4x4 gWorld;
}

VertexOut main(VertexIn Input)
{
    VertexOut output;
    output.Position = mul(float4(Input.Position, 1.f), gWorldViewProj);
    output.WorldNormal = mul(float4(Input.Normal, 0.f), gWorld).xyz;
    output.Color = Input.Color;
    
    return output;
}