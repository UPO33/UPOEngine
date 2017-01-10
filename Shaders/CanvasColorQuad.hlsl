#include "Common.hlsl"


struct VSIn
{
    float2 position : POSITION;
    float4 color : COLOR;
};
struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VSOut VSMain(VSIn input)
{
    VSOut output = (VSOut) 0;
    //output.pos = UV2NDC(input.position);
    output.pos = UV2NDC(input.position);
    output.color = input.color;
    return output;
}
struct PSOut
{
	float4	color : SV_Target0;
	uint id : SV_Target1;
};
PSOut PSMain(VSOut input)
{
	PSOut output;
	output.color = input.color;
	output.id = 0;
	return output;
}