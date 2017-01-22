#include "DeferredCommon.hlsl"


cbuffer CBPerDraw : register(b2)
{
    matrix gLocalToWorld;
};

struct VSIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct PSIn
{
    float4 position : SV_Position;
    float3 positionWS : WORLDPOS;
    float2 uv : UV;
};



Texture2D gDiffuseTexture : register(t0);
SamplerState gDiffuseSampler : register(s0);




void VSMain(in VSIn input, out PSIn output)
{
    float4 worldPos = mul(gLocalToWorld, float4(input.position, 1));
    output.positionWS = worldPos;
    output.position = mul(gCamera.mWorldToCilp, worldPos);
    output.uv = input.uv;
};

void PSMain(in PSIn input,
    out float4 gbufferA : SV_Target0,
    out float4 gbufferB : SV_Target1)
{
    GBufferData gbuffer;

    gbuffer.WorldPos = input.positionWS;
    gbuffer.DiffuseColor = float3(1, 1, 1); //gDiffuseTexture.Sample(gDiffuseSampler, input.uv);

    EncodeGBuffer(gbuffer, gbufferA, gbufferB);
};