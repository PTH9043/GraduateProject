#ifndef _DEFFERED_HLSL_
#define _DEFFERED_HLSL_

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

PS_OUT PS_Main(PS_In Input)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture0.Sample(g_Sampler_Normal, Input.vTexUV);
    //if(Out.vColor.a == 0)
    //    discard;
    
    Out.vColor += g_Texture1.Sample(g_Sampler_Normal, Input.vTexUV);
    //Out.vColor += g_Texture2.Sample(g_Sampler_Normal, Input.vTexUV);
    return Out;
}

#endif // _DEFFERED_HLSL_