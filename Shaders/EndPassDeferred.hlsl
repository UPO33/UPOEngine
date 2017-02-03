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

void VSMain(in uint vid : SV_VertexID, out PSInEndPass output)
{
    DrawRect(vid, output.position, output.uv); 
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

float4 PSMain(PSInEndPass input) : SV_Target
{
#ifdef VISALIZE_GBUFFER
    float4 outColor = 0;
    {
        const float2 VisSize = float(0.2);
        const float VisSpace = 0;

        float2 visOffset = float2(0, 0);
        
        //depth
        {
            float4 depthSampled = 0;
            DrawVisRect(gDepth, input.uv, visOffset, VisSize, depthSampled);
            outColor = GetLinearDepth(depthSampled.x, gCamera.mProjection) / 10.0f;
        }
        //position
        {
            visOffset.x += VisSpace + VisSize.x;
            DrawVisRect(gGBufferA, input.uv, visOffset, VisSize, outColor);
            outColor /= 100;
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

