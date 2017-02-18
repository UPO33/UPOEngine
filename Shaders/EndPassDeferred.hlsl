#include "DeferredCommon.hlsl"


Texture2D gGBufferA : register(t0);
Texture2D gGBufferB : register(t1);
Texture2D gGBufferC : register(t2);
Texture2D gDepth : register(t3);
Texture2D gIDBuffer : register(t4);

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

    float4 vv = clipPos;
    vv.z = 1;
    float4 v0 = mul(gCamera.mClipToWorld, vv);
    fPos = v0.xyz / v0.w;
}

float2 asd(uint2 v, uint size)
{
    v / size;
}
float2 MapRenderTargetUV(float2 uvToSmaple, uint2 renderedTextureSize, uint2 originalTextureSize)
{
    return uvToSmaple * renderedTextureSize / originalTextureSize;
}

bool DrawVisRect(Texture2D tx, float2 uv, float2 VisUVPos, float2 visUVSize, in out float4 outColor)
{
    const float BorderSize = 0.01f;

    if (all(uv >= VisUVPos) && all(uv <= VisUVPos + visUVSize))
    {
        float2 sampleUV = (uv - VisUVPos) / visUVSize;
        outColor = tx.Load(int3(sampleUV * gCamera.mViewportSize, 0));
        return true;
    }

    return false;
}

float4 PSMain(PSInEndPass input, float3 fpos : FPOS) : SV_Target
{
    float3 cameraPos = GetCameraWorldPos(gCamera);
    float3 toFrustum = normalize(fpos - cameraPos);
    

#ifdef VISALIZE_GBUFFER



    float4 outColor = 0;
    {
        const float2 VisSize = float(0.2);
        const float VisSpace = 0;

        float2 visOffset = float2(0, 0);
        
        
        //depth
        {
            float4 depthSampled = 0;
            DrawVisRect(gGBufferA, input.uv, visOffset, VisSize, depthSampled);
            //outColor = GetLinearDepth(depthSampled.x, gCamera.mProjection) / 100.0f;
            outColor = depthSampled.w / 100.0f;
        }
        //position
        {
            

            visOffset.x += VisSpace + VisSize.x;
            float2 uv = input.uv;

            if (all(uv >= visOffset) && all(uv <= visOffset + VisSize))
            {
                float2 sampleUV = (uv - visOffset) / VisSize;
                float3 worldPos = gGBufferA.Load(uint3(sampleUV * gCamera.mViewportSize, 0)).xyz;
                outColor.xyz = worldPos / 400.0;
                outColor.w = 1;
            }

            /*
            visOffset.x += VisSpace + VisSize.x;
            float2 uv = input.uv;

            if (all(uv >= visOffset) && all(uv <= visOffset + VisSize))
            {
                float2 sampleUV = (uv - visOffset) / VisSize;
                float zz = gDepth.Load(uint3(sampleUV * gCamera.mViewportSize, 0)).x;
                float xx = input.uv.x * 2 - 1;
                float yy = (1 - input.uv.y) * 2 - 1;
                float4 ndc = float4(xx, yy, zz, 1);

                float4 pp = mul(gCamera.mClipToWorld, ndc);
                float3 worldPos = pp.xyz / pp.w;
                outColor.xyz = worldPos / 100.0;
                outColor.w = 1;
            }
            */
            

        }
        {
            visOffset.x += VisSpace + VisSize.x;
            DrawVisRect(gGBufferB, input.uv, visOffset, VisSize, outColor);
        }
        {
            visOffset.x += VisSpace + VisSize.x;
            DrawVisRect(gGBufferC, input.uv, visOffset, VisSize, outColor);
        }
        {
            visOffset.x += VisSpace + VisSize.x;
            float2 uv = input.uv;

            if (all(uv >= visOffset) && all(uv <= visOffset + VisSize))
            {
                float2 sampleUV = (uv - visOffset) / VisSize;
                uint idSampled = gIDBuffer.Load(int3(sampleUV * gCamera.mViewportSize, 0)).x;
                float f = float(idSampled / 10.0);
                outColor = float4(idSampled / 3.0, idSampled / 33.0, idSampled / 100.0, 1);
            }

        }
        visOffset.xy = float2(0, VisSize.y);
        DrawVisRect(gGBufferB, input.uv, visOffset, float2(1.0, 1.0 - visOffset.y), outColor);
    }

    return outColor;
#else

    int3 vSamplePoint = uint3((uint2) input.position.xy, 0);
    float4 gbufferA = gGBufferA.Load(vSamplePoint);
    float4 gbufferB = gGBufferB.Load(vSamplePoint);
    float4 gbufferC = gGBufferC.Load(vSamplePoint);

    return gbufferB.rgba;
    /*
    GBufferData gbData;
    DecodeGBuffer(gbufferA, gbufferB, gbufferC, gbData);
    return float4(gbData.DiffuseColor, 0);
    */

#endif

};

