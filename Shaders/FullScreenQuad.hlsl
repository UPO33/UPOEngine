
struct VSOut
{
    float4 position : SV_Position;
    float2 uv : UV;
};

VSOut VSMain(uint id : SV_VertexID)
{
    VSOut output;
    output.position.x = (float) (id / 2) * 4.0 - 1.0f;
    output.position.y = (float) (id % 2) * 4.0 - 1.0f;
    output.position.z = 0;
    output.position.w = 1;

    output.uv.x = (float) (id / 2) * 2.0;
    output.uv.y = 1.0 - (float) (id % 2) * 2;
    return output;
}

Texture2D gColorMap : register(t0);
Texture2D gIDMap : register(t1);
SamplerState gPointSampler : register(s0);

float4 VSMain(VSOut input) : SV_Target
{
    
}