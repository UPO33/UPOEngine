#ifndef DEFFERD_COMMON
#define DEFFERD_COMMON

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
    float3 positionWS : WORLDPOS;
    float3 normal : NORMAL;
    float2 uv : UV;
};


struct GBufferData
{
    float3 WorldPos;
    float3 DiffuseColor;
    float3 Normal;
    float depth;
};

cbuffer CBPerFrame : register(b0)
{
    PerFrameData gFrame;
}
    
cbuffer CBPerCamera : register(b1)
{
    PerCameraData gCamera;
};

cbuffer CBPerObject : register(b2)
{
    PerObjectData gObject;
};

#ifdef USE_HITSELECTION
cbuffer CBHitSelection : register(b3)
{
    uint    gHitID;
};
#endif

struct GBufferTargets
{
    float4 gbufferA : SV_Target0;
    float4 gbufferB : SV_Target1;
    float4 gbufferC : SV_Target2;
#ifdef USE_HITSELECTION
    uint4 hitID : SV_Target3;
#endif
};


#define MaterialTextureSlot t3
#define MaterialSamplerSlot s3
#define MaterialCBufferSlot b4


void EncodeGBuffer(in GBufferData input, out float4 gbufferA, out float4 gbufferB, out float4 gbufferC)
{
    gbufferA = float4(input.WorldPos, input.depth);
    gbufferB = float4(input.DiffuseColor, 0);
    gbufferC = float4(input.Normal * 0.5 + 0.5, 0);
}
void EncodeGBuffer(in GBufferData input, out GBufferTargets output)
{
    output.gbufferA = float4(input.WorldPos, input.depth);
    output.gbufferB = float4(input.DiffuseColor, 0);
    output.gbufferC = float4(input.Normal * 0.5 + 0.5, 0);
}
void DecodeGBuffer(in float4 gbufferA, in float4 gbufferB, in float4 gbufferC, out GBufferData output)
{
    output.WorldPos = gbufferA.xyz;
    output.depth = gbufferA.w;
    output.DiffuseColor = gbufferB.xyz;
    output.Normal = normalize(gbufferC.xyz * 2 - 1);
}

#endif //DEFFERD_COMMON