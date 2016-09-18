#include "Common.hlsl"

struct VSIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct PSIn
{
    float4 position : SV_Position;
    float3 normalWS : NORMAL;
    float3 positionWS : WORLDPOS;
    float2 uv : UV;
};

struct LighInfo
{
    float3 position;
    float radius;
    float3 direction;
    float spotAngle;
    float3 intensity;
};



cbuffer CBPerObject : register(b1)
{
    matrix gWorld;
};


Texture2D gDiffuseTexture : register(t0);
SamplerState gDiffuseSampler : register(s0);


PSIn VSMain(VSIn input)
{
    PSIn output = (PSIn) 0;
    float4 worldPos = mul(float4(input.position, 1), gWorld);
    output.position = mul(worldPos, gView);
    output.position = mul(output.position, gProj);
    output.uv = input.uv;
    return output;
}

float4 PSMain(PSIn input) : SV_Target
{
    return float4(1, 1, 1, 1);
}