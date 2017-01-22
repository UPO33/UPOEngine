#include "Common.hlsl"

Texture2D gPositionMap : register(t0);
Texture2D gDiffuseMap : register(t1);
SamplerState gSampler : register(s0);

struct PSIn
{
    float4 position : SV_Position;
    float2 uv : UV;
};

void VSMain(in uint vid : SV_VertexID,   out PSIn output)
{
    DrawRect(vid, output.position, output.uv); 
}

float2 asd(uint2 v, uint size)
{
    v / size;
}

float4 PSMain(PSIn input) : SV_Target
{

   // return float4(input.uv, 0, 0);
    float w, h;
    gDiffuseMap.GetDimensions(w, h);

    return gDiffuseMap.Load(uint3((uint2)input.position.xy, 0));
};

