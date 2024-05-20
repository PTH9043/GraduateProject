#ifndef _HORIZONTALBLUR_HLSL_
#define _HORIZONTALBLUR_HLSL_

#include "ShaderGrobalFunc.hlsli"


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = Compute_FinalMatrix(In.vPosition);
    Out.vTexUV = In.vTexUV;

    return Out;
}

struct PS_In
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_Target0;
};

const float TexWidth = 1280;
const float TexHeight = 1080;

static const float Weight[13]=
{
 0.0561,0.1353,0.278,0.4868,0.7261,0.9231,1,
0.9231,0.7261,0.4868,0.278,0.1353,0.0561
};

static const float Total = 6.2108;

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 originalUV = In.vTexUV;
    float2 uv=0;
    
    float TexUv = 1.f / (TexHeight/2.f);
    
    for (int i = -6; i < 6; ++i)
    {
        uv = originalUV + float2(0, TexUv * i);
        Out.vColor += Weight[6 * i] * g_Texture0.Sample(g_Sampler_Clamp, uv);
    }
    Out.vColor /= Total;

        return Out;
}

#endif // _HORIZONTALBLUR_HLSL_