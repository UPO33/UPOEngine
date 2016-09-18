#include "Common.hlsl"

struct VSIn
{
	float3 position : POSITION; 
};

struct PSIn
{
	float4 position : SV_Position;
    float3 worldPosition : WPOS;
};

PSIn VSMain(VSIn input)
{
	PSIn output = (PSIn)0;
	
    float3 camOffset = float3(gCameraPosition.x, 0, gCameraPosition.z);
    output.worldPosition = input.position + camOffset;
	output.position = mul(float4(output.worldPosition, 1), gViewProj);
	
	return output;
}

float3 PSMain(PSIn input) : SV_Target
{
    
}