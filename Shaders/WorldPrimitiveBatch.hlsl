#include "DeferredCommon.hlsl"

/*
cbuffer CBGlobal : register(b0)
{
    matrix gWorldToCLip;
};
*/

struct VSIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PSIn
{
    float4 position : SV_Position;
    float4 color : COLOR;
};


void VSMain(in VSIn input, out PSIn output)
{
    
#if 0
    output.position = mul(float4(input.pos, 1), gWorldToCLip);
#else
    output.position = mul(gCamera.mWorldToCilp, float4(input.pos, 1));
#endif
    output.color = input.color;
    //return mul(float4(pos, 1), gWorldToCLip);
}

float4 PSMain(PSIn input) : SV_Target
{
    return input.color;
}
