#ifndef _INSTANCERECT_HLSL_
#define _INSTANCERECT_HLSL_

#include "ShaderGrobalFunc.hlsli"

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float4x4 mInstanceTransform : WORLDMATRIX;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION0;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_Main(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), In.mInstanceTransform);
	Out.vPosition = Compute_FinalMatrix(Out.vPosition);
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

PS_OUT PS_Main(PS_In Input)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture0.Sample(g_Sampler_Normal, Input.vTexUV);

    return Out;
}

#endif // _INSTANCERECT_HLSL_