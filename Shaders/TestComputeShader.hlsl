
struct AABB
{
    float4 min;
    float4 max;
};

struct Convex
{
    float4 planes[6];
};

StructuredBuffer<Convex> gConvexs;
StructuredBuffer<AABB> gBounds;