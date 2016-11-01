#include "Common.hlsl"


struct VSIn
{
    float2 position : POSITION;
    uint vid : SV_VertexID;
};
struct VSOut
{
    float4 pos : SV_Position;
};
static const float2 poss[] = { float2(0, 0), float2(1, 0), float2(0, 1) };

VSOut VSMain(VSIn input)
{
    VSOut output = (VSOut) 0;
    //output.pos = UV2NDC(input.position);
    output.pos = UV2NDC(poss[input.vid]);
    return output;
}
float4 PSMain(VSOut input) : SV_Target
{
    return TEST_COLOR;
}