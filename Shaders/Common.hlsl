#ifndef COMMON
#define COMMON

/*
Interpolation Modifier	Description

[linear]
Interpolate between shader inputs; linear is the default value if no interpolation modifier is specified.

[centroid]	
Interpolate between samples that are somewhere within the covered area of the pixel (this may require extrapolating end points from a pixel center).
Centroid sampling may improve antialiasing if a pixel is partially covered (even if the pixel center is not covered).
 The centroid modifier must be combined with either the linear or noperspective modifier.

[nointerpolation]
Do not interpolate

[noperspective]

Do not perform perspective-correction during interpolation. The noperspective modifier can be combined with the centroid modifier.
sample	Available in shader model 4.1 and later
Interpolate at sample location rather than at the pixel center. This causes the pixel shader to execute per-sample
rather than per-pixel. Another way to cause per-sample execution is to have an input with semantic SV_SampleIndex,
which indicates the current sample. Only the inputs with sample specified (or inputting SV_SampleIndex) differ
between shader invocations in the pixel, whereas other inputs that do not specify modifiers (for example, 
if you mix modifiers on different inputs) still interpolate at the pixel center. Both pixel shader invocation
and depth/stencil testing occur for every covered sample in the pixel. This is sometimes known as supersampling.
In contrast, in the absence of sample frequency invocation, known as multisampling, the pixel shader is invoked
once per pixel regardless of how many samples are covered, while depth/stencil testing occurs at sample frequency.
Both modes provide equivalent edge antialiasing. However, supersampling provides better 
shading quality by invoking the pixel shader more frequently.
*/

struct PerFrameData
{
    float4 mSunDir;
    float4 mSunColor;
    float4 mRandomColor;
};

struct PerCameraData
{
    matrix mProjection;
    matrix mInvProjection;
    matrix mView;
    matrix mInvView;
    matrix mWorldToCilp;
    matrix mClipToWorld;
    float3 mWorldPosition;
    float padding0;
};



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

#endif