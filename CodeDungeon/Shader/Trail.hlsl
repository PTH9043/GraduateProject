#ifndef _TRAIL_HLSL_
#define _TRAIL_HLSL_

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

cbuffer HasNormalBuffer : register(b14)
{
    float4 gf4Color;
};

VS_OUT VS_Main(VS_IN In) //이름 VS_Main PS_Main 유지.
{
	VS_OUT Out = (VS_OUT)0;

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

    float4 cDistortion = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV);
    
    float4 cColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV + (cDistortion.r * 2.f));
    
    cColor.a = In.vTexUV.y;
	
   // float4 cMulColor = { 1.f / 255.f, 165.f / 255.f, 172.f / 255.f, 0.f };
  //  cColor += cMulColor;
    cColor += gf4Color;
    Out.vColor = cColor;
    
    return Out;
}

#endif // _TRAIL_HLSL_