#include "DeferredCommon.hlsl"


struct PerMaterialData
{
    float4 mColor;
    float2 mUVOffset;
    float2 mUVScale;
};
cbuffer CBPerMaterial : register(MaterialCBufferSlot)
{
    PerMaterialData gMaterial;
};

Texture2D gDiffuseTexture : register(MaterialTextureSlot);
SamplerState gDiffuseSampler : register(MaterialSamplerSlot);



//matrix combination in hlsl  mul(mul(Proj * mView), mWorld);

void VSMain(in VSIn input, out PSIn output, out float4 outClipSpace : TC0)
{
    float4 worldPos = mul(gObject.mWorldMatrix, float4(input.position, 1));
    output.positionWS = worldPos;
    output.normal = mul((float3x3) gObject.mWorldMatrix, input.normal);
    float4x4 mat = mul(mul(gCamera.mProjection, gCamera.mView), gObject.mWorldMatrix);
    output.position = mul(mat, float4(input.position, 1));
    //output.position = mul(gCamera.mWorldToCilp, worldPos);
    outClipSpace = output.position;
#ifdef APPLY_UV_TRANSFORMATION
    output.uv = input.uv * gMaterial.mUVScale + gMaterial.mUVOffset;
#else
	output.uv = input.uv;
#endif
};

void PSMain(in PSIn input, in float4 clipSpace : TC0,
    out float4 gbufferA : SV_Target0,
    out float4 gbufferB : SV_Target1,
    out float4 gbufferC : SV_Target2
#ifdef USE_HITSELECTION
    ,out uint4 outHitID : SV_Target3
#endif
)
{
    GBufferData gbuffer;

    gbuffer.depth = clipSpace.z;
    gbuffer.WorldPos = input.positionWS;
    gbuffer.Normal = normalize(input.normal);
    gbuffer.DiffuseColor = (gDiffuseTexture.Sample(gDiffuseSampler, input.uv)).rgb;

#ifdef USE_HITSELECTION
    outHitID = gHitID;
#endif

    EncodeGBuffer(gbuffer, gbufferA, gbufferB, gbufferC);
};