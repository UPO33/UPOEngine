#include "Common.hlsl"

struct VSIn
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PSIn
{
    float4 position : SV_Position;
    /*nointerpolation*/ float4 color : COLOR;
};

PSIn VSMain(VSIn input)
{
    PSIn output = (PSIn) 0;
    /*
    float4 viewPos = mul(float4(input.position, 1), gView);
    output.position = mul(viewPos, gProj);
    */
    output.color = input.color;
   
    output.position = mul(float4(input.position, 1), gViewProj);
    return output;
}

float4 PSMain(PSIn input) : SV_Target
{
    return input.color;
}
