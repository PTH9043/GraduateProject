#ifndef _NORMALOBJECT_HLSL_
#define _NORMALOBJECT_HLSL_

#include "ShaderGrobalFunc.hlsli"

struct WATERSTRUCT
{
    float2 vWaterNormalMapOffset; // 물 표면의 노말 맵 이동 오프셋
    float fWaveAmplitude; // 파고
    float fWaveFrequency; // 파동 빈도
    float fWaveSpeed; // 파동 속도
    
    float Padding;
};

cbuffer WATEROBJECTDATA : register(b14)
{
    WATERSTRUCT   waterStruct;
    matrix                   g_mTextureAnimation;
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
    float4 vPosition : SV_POSITION0;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vPosition : SV_Target3;
};

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 uv = mul(float3(In.vTexUV, 1.0f), (float3x3) g_mTextureAnimation).xy;

    float4 cBaseTexColor = g_Texture0.SampleLevel(g_Sampler_Normal, uv, 0);
    float4 cDetail0TexColor = g_Texture1.SampleLevel(g_Sampler_Normal, uv * 20.0f, 0);
    float4 cDetail1TexColor = g_Texture2.SampleLevel(g_Sampler_Normal, uv * 20.0f, 0);     

    
    Out.vDiffuse = lerp(cBaseTexColor * cDetail0TexColor, cDetail1TexColor.r * 0.5f, 0.35f);
    Out.vNormal = (float4) 1.f;
    Out.vDepth = float4(0.f, 0.f, 0.f, 0.f);
    return Out;
}
#endif // _NORMALOBJECT_HLSL_