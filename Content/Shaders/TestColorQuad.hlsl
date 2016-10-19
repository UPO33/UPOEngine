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

struct VSIn
{
    float2 position : POSITION;
};
struct VSOut
{
    float4 pos : SV_Position;
};

VSOut VSMain(VSIn input)
{
    VSOut output = (VSOut) 0;
    output.pos = UV2NDC(input.position);
    return output;
}
float4 PSMain(VSOut input) : SV_Target
{
    return float4(1, 0, 0, 1);
}