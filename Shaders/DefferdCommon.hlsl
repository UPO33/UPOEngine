#ifndef DEFFERD_COMMON
#define DEFFERD_COMMON

#include "Common.hlsl"

struct GBufferData
{
    float3 WorldPos;
    float3 DiffuseColor;
};


cbuffer CBPerFrame : register(b0)
{
    PerFrameData gFrame;
}
    
cbuffer CBPerCamera : register(b1)
{
    PerCameraData gCamera;
};



void EncodeGBuffer(in GBufferData input, out float4 gbufferA, out float4 gbufferB)
{
    gbufferA = float4(input.WorldPos, 0);
    gbufferB = float4(input.DiffuseColor, 0);
}

void DecodeGBuffer(in float4 gbufferA, in float4 gbufferB, out GBufferData output)
{
    output.WorldPos = gbufferA.xyz;
    output.DiffuseColor = gbufferB.xyz;
}

#endif //DEFFERD_COMMON