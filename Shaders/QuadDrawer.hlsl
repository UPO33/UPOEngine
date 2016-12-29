
float4 UV2NDC(float2 uv)
{
#if 1
	float2 xy = uv * 2 - 1;
	return float4(xy.x, -xy.y, 0, 1);
#else 
    float2 xy = uv - 0.5;
    return float4(xy.x, -xy.y, 0, 0.5f);
#endif
}
float2 Clip2UV(float4 v)
{
    return v.xy / v.ww * float2(0.5, -0.5) + 0.5;
}


struct VSOut
{
	float4 position : SV_Position;
};
cbuffer PerElement : register(b0)
{
    float4 gPositions[4];   //uv space
    float4 gColor;
};

Texture2D gTexture : register(t0);
SamplerState gLinearSampler : register(s0);

VSOut VSMain(uint vi : SV_VertexID)
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
    float4 color = gTexture.Sample(gLinearSampler, input.uv);

       //clip(color.a - 0.1);
    return color;
}