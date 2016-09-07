
float4 UV2NDC(float2 uv)
{
#if 0
	float2 xy = uv * 2 - 1;
	return float4(xy.x, -xy.y, 0, 1);
#else 
    float2 xy = uv - 0.5;
    return float4(xy.x, -xy.y, 0, 0.5f);
#endif
}

struct VSIn
{
	float2 position : POSITION;
    float2 uv : UV;
    uint id : SV_VertexID;
};
struct VSOut
{
	float4 position : SV_Position;
    float2 uv : UV;
    float4 color : COLOR;
};
cbuffer PerElement : register(b0)
{
    float Time;
};

Texture2D colorMap : register(t0);
SamplerState linearSampler : register(s0);

VSOut VSMain(VSIn input)
{
	VSOut output = (VSOut)0;
	output.position = UV2NDC(input.position);
    output.uv = input.uv;
    float4 colors[] = { float4(1, 0, 0, 0), float4(0, 1, 0, 0), float4(0, 0, 1, 0) };
    output.color = colors[input.id % 3];
	return output;
}
float4 PSMain(VSOut input) : SV_Target
{
    //return float4(1,0,1,1);
	return colorMap.Sample(linearSampler, input.uv);
}