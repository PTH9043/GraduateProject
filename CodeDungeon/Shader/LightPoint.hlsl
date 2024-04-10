#ifndef _LIGHTPOINT_HLSL_
#define _LIGHTPOINT_HLSL_

#include "LightShaderFunc.hlsli"

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

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_tLightParam.mScreenWorldMatrix);
    Out.vPosition = mul(Out.vPosition, g_tLightParam.mScreenViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_tLightParam.mScreenProjMatrix);
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
    float4 vAmbient : SV_TARGET0;
    float4 vShade : SV_TARGET1;
    float4 vSpecular : SV_TARGET2;
};

PS_OUT PS_Main(PS_In Input)
{
    PS_OUT Out = (PS_OUT) 0;

    // 0, 1, 3
    float4 vNormal = g_Texture0.Sample(g_Sampler_Normal, Input.vTexUV);
    vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 0.f));
    float4 vDepth = g_Texture1.Sample(g_Sampler_Normal, Input.vTexUV);
    float4 vPosition = g_Texture2.Sample(g_Sampler_Normal, Input.vTexUV);

    float4 vViewPosition = mul(vPosition, g_tLightParam.mViewMatrix);
    float4 vViewNormal = mul(vNormal, g_tLightParam.mViewMatrix);
    vViewNormal = normalize(vViewNormal);
    
    LIGHTCOLOR tLightColor = CalculateLightColor(vViewNormal.xyz, vViewPosition.xyz, vNormal.xyz, vPosition.xyz);
    Out.vAmbient = tLightColor.vAmbient;
    Out.vShade = tLightColor.vDiffuse;
    Out.vSpecular = tLightColor.vSpecular + RimLight(vPosition, vNormal);

	return Out;
}

#endif // _LIGHTDEFFERED_HLSL_