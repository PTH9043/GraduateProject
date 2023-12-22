#ifndef _3DCUBE_HLSL_
#define _3DCUBE_HLSL_

#include "ShaderGrobalFunc.hlsli"

struct VS_IN
{
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vTexUV : TEXCOORD0;
};

VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    Out.vPosition = float4(In.vPosition, 0);
    Out.vPosition = mul(Out.vPosition, GetViewProjInfo().mViewMatrix);
    Out.vPosition = mul(Out.vPosition, GetViewProjInfo().mProjMatrix);
    
    Out.vPosition = Out.vPosition.xyww;
    Out.vTexUV = In.vPosition;
    return Out;
}

struct PS_In
{
    float4 vPosition : SV_POSITION0;
    float3 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vPosition : SV_Target3;
};

PS_OUT PS_Main(PS_In Input)
{
    PS_OUT Out = (PS_OUT) 0;

    // Texture3D SkyBoxTexture
    Out.vDiffuse = g_Texture9.Sample(g_Sampler_Clamp, Input.vTexUV);
    Out.vNormal = (float4) 1.f;
    Out.vDepth = float4(0.f, 0.f, 0.f, 0.f);
    return Out;
}

#endif