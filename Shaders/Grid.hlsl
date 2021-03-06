#include "DeferredCommon.hlsl"

/*
GRID_MAX_DIST
*/
struct VSInGrid
{
	float3 position : POSITION;
    float4 color : COLOR;
};

struct PSInGrid
{
	float4 position : SV_Position;
    float3 worldPosition : WPOS;
    float4 color : COLOR;
};

void VSMain(in VSInGrid input, out PSInGrid output)
{
    //float3 camOffset = float3(gCamera.mWorldPosition.x, 0, gCamera.mWorldPosition.z);
    output.worldPosition = input.position;
    output.position = mul(gCamera.mWorldToCilp, float4(output.worldPosition, 1));
    output.color = input.color;
}

float4 PSMain(in PSInGrid input) : SV_Target
{
    float dist = length(input.worldPosition - GetCameraWorldPos(gCamera));
    float a =  1 - saturate(dist / 800);
    return float4(input.color.rgb, a);
}