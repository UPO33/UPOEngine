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

cbuffer CBPerFrame : register(b0)
{
    matrix gProj;
    matrix gView;
    matrix gViewProj;
    float3 gCameraForward;
    float gTime;
    float3 gCameraPosition;
    float4 gAmbientColor;
    float4 gSunColor;
    float3 gSunDir;
    float gPadding0;

};

#endif