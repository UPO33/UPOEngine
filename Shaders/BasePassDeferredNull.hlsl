#include "DeferredCommon.hlsl"



void VSMain(in VSIn input, out PSIn output)
{
    float4 worldPos = mul(gObject.mWorldMatrix, float4(input.position, 1));
    output.positionWS = worldPos;
    output.normal = mul((float3x3) gObject.mWorldMatrix, input.normal);
    output.position = mul(gCamera.mWorldToCilp, worldPos);
    output.uv = input.uv;
};

void PSMain(in PSIn input,
    out float4 gbufferA : SV_Target0,
    out float4 gbufferB : SV_Target1,
    out float4 gbufferC : SV_Target2)
{
    GBufferData gbuffer;

    gbuffer.depth = 0;
    gbuffer.WorldPos = input.positionWS;
    gbuffer.Normal = normalize(input.normal);
    gbuffer.DiffuseColor = float3(1, 1, 1);

    EncodeGBuffer(gbuffer, gbufferA, gbufferB, gbufferC);
};