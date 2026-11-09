#include "BaseShadertypes.hlsl"

static float3 gLightDir = float3(0.57735f, -0.57735f, 0.57735f);

float4 main(VertexOut input) : SV_TARGET
{
    return saturate(dot(input.WorldNormal, -gLightDir)) * input.Color;
}