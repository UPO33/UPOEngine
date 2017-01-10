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
float4 ScreenToNDC(float2 screenPos, float2 screenSize)
{
    return UV2NDC(screenPos / screenSize);
}

float2 Clip2UV(float4 v)
{
    return v.xy / v.ww * float2(0.5, -0.5) + 0.5;
}

#ifdef USE_UVSPACE
#define TRANSFORM_SPACE(v)   UV2NDC(v);
#else   
#define TRANSFORM_SPACE(v) float4(v, float2(0, 1));
#endif


struct VSIn
{
    float2 position : POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

struct VSOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : UV;
};

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0)
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

VSOut VSMain(VSIn input)
{
    VSOut output = (VSOut) 0;
    output.position = TRANSFORM_SPACE(input.position);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}
float4 PSMain(VSOut input) : SV_Target
{
    float4 color = gTexture.Sample(gSampler, input.uv) * input.color;
    return color;
}