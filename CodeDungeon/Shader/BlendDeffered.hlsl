#ifndef _NONALPHADEFFERED_HLSL_
#define _NONALPHADEFFERED_HLSL_

#include "ShaderGrobalFunc.hlsli"

cbuffer BLENDCOLOR : register(b3)
{
    float4 g_RectColor;
    float4 g_Default;
};

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

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuseDesc = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
    vector vShadeDesc = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV);
    vector vSpecularDesc = g_Texture2.Sample(g_Sampler_Normal, In.vTexUV);
    vector vAmbientDesc = g_Texture3.Sample(g_Sampler_Normal, In.vTexUV);

    vector LightCol = (vShadeDesc + vAmbientDesc + vSpecularDesc);
    LightCol = clamp(LightCol, 0, 1);
    
    vector vColor = vDiffuseDesc /** LightCol*/;
    
    //vector vColor = lerp(vDiffuseDesc, (vShadeDesc + vAmbientDesc + vSpecularDesc), 0.5f);
    
    Out.vColor = vColor;
 
    Out.vColor.a = vDiffuseDesc.a;
    return Out;
}

#endif // _NONALPHADEFFERED_HLSL_