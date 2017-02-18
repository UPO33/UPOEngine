#include "DeferredCommon.hlsl"


Texture2D gGBufferA : register(t0);
Texture2D gGBufferB : register(t1);
Texture2D gGBufferC : register(t2);
Texture2D gDepth : register(t3);

SamplerState gSampler : register(s0);

struct PSInEndPass
{
    float4 position : SV_Position;
    float2 uv : UV;
};

void VSMain(in uint vid : SV_VertexID, out PSInEndPass output, out float3 fPos : FPOS)
{
    float4 clipPos = 0;
    float2 uv = 0;
    DrawRect(vid, clipPos, uv); 
    output.uv = uv;
    output.position = clipPos;

}



float4 PSMain(PSInEndPass input, float3 fpos : FPOS) : SV_Target
{
    int3 vSamplePoint = uint3((uint2) input.position.xy, 0);
    float4 gbufferA = gGBufferA.Load(vSamplePoint);
    float4 gbufferB = gGBufferB.Load(vSamplePoint);
    float4 gbufferC = gGBufferC.Load(vSamplePoint);

    GBufferData gbData;
    DecodeGBuffer(gbufferA, gbufferB, gbufferC, gbData);

    float ndl = max(dot(gbData.Normal, -gFrame.mSunDir.xyz), 0);
};

