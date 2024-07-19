#ifndef _DEFAULTUI_HLSL_
#define _DEFAULTUI_HLSL_

#include "ShaderGrobalFunc.hlsli"


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION0;
    float2 vTexUV : TEXCOORD0;
};

cbuffer ButtonTimerBuffer : register(b13)
{
    float3 _vButtonColor;
    float _durationTime;
};


VS_OUT VS_Main(VS_IN In) //이름 VS_Main PS_Main 유지.
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = Compute_FinalMatrix(In.vPosition);
    Out.vTexUV = In.vTexUV;

    return Out;
}

struct PS_In
{
    float4 vPosition : SV_POSITION0;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 texColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
    
    float progress = frac(_durationTime); 
    float borderSize = 0.01; 

    float2 center = float2(0.5, 0.5); 
    float2 dir = normalize(In.vTexUV - center);
    float angle = atan2(dir.y, dir.x) / (2 * 3.14159) + 0.5; 

   
    if (length(In.vTexUV - center) > (0.5 - borderSize) && angle < progress)
    {
        Out.vColor = float4(_vButtonColor, 1.0);
    }
    else
    {
        Out.vColor = texColor;
    }

    return Out;
}

#endif // _RECT_H_DEFAULTUI_HLSL_LSL_