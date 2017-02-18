#include "DeferredCommon.hlsl"



struct VSInPB
{
    float3 position : POSITION;
    float4 color : COLOR;
    uint hitID : HITID;
};

struct PSInPB
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float3 worldPos : WORLDPOS;
    nointerpolation uint hitID : HITID;
};


void VSMain(in VSInPB input, out PSInPB output)
{
    output.position = mul(gCamera.mWorldToCilp, float4(input.position, 1));
    output.color = input.color;
    output.hitID = input.hitID;
    output.worldPos = input.position;
}

void PSMain(PSInPB input
    ,out float4 gbufferA : SV_Target0
    ,out float4 gbufferB : SV_Target1
    ,out float4 gbufferC : SV_Target2
#ifdef USE_HITSELECTION
    ,out uint4 outHitID : SV_Target3
#endif
)
{
    GBufferData gbdata;
    gbdata.depth = 0;
    gbdata.Normal = float3(0, 0, 1);
    gbdata.DiffuseColor = input.color;
    gbdata.WorldPos = input.worldPos;
    
    EncodeGBuffer(gbdata, gbufferA, gbufferB, gbufferC);

#ifdef USE_HITSELECTION
    outHitID = input.hitID;
#endif
    
}

struct VSInSphereDraw
{
    float3 WorldPos : POSITION;
    float Radius: RADIUS;
    float4 Color : COLOR;
    uint HitID : HITID;

};

void VSWireSphere(in VSInSphereDraw input, out VSInSphereDraw output)
{
    output = input;
}


void VSWireCircle(
    in float3 position : POSITION,
    in float4 color : COLOR,
    in matrix local2World : LOCAL2WORLD,
    in uint hitID : HITID,
    out PSInPB output)
{
    output.color = color;
    output.hitID = hitID;
    float4 worldPos = mul(local2World, float4(position, 1));
    output.worldPos = worldPos.xyz;
    output.position = mul(gCamera.mWorldToCilp, worldPos);
}



[maxvertexcount(61)]
void GSWireSphere(point VSInSphereDraw input[1], inout LineStream<PSInPB> outStream)
{
 
    for (uint i = 0; i < 61; i++)
    {
        const float offset = 360.0 / 60.0;

        float s = 0;
        float c = 0;
        sincos(radians(i*offset), s, c);

        float3 worldPos = input[0].WorldPos + float3(s, 0, c) * input[0].Radius;
        float4 clipPos = mul(gCamera.mWorldToCilp, float4(worldPos, 1));
        PSInPB ret;
        ret.worldPos = worldPos;
        ret.position = clipPos;
        ret.color = input[0].Color;
        ret.hitID = input[0].HitID;

        outStream.Append(ret);
    }
    /*
    PSInPB ret;
    ret.worldPos = 0;
    ret.position = float4(0,0,0,1);
    ret.color = input[0].Color;
    ret.hitID = input[0].HitID;

    outStream.Append(ret);

    ret.worldPos = 0;
    ret.position = (1,1,0,1);

    outStream.Append(ret);
  */
};

