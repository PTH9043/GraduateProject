#ifndef _MODEL_HLSL_
#define _MODEL_HLSL_

#include "ShaderGrobalFunc.hlsli"

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV0 : TEXCOORD0;
    float2 vTexUV1 : TEXCOORD1;
    float2 vTexUV2 : TEXCOORD2;
    float2 vTexUV3 : TEXCOORD3;
    float2 vTexUV4 : TEXCOORD4;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    float2 vTexUV0 : TEXCOORD0;
    float2 vTexUV1 : TEXCOORD1;
    float2 vTexUV2 : TEXCOORD2;
    float2 vTexUV3 : TEXCOORD3;
    float2 vTexUV4 : TEXCOORD4;
    float4 vWorldPos : TEXCOORD5;
    float4 vProjPos : TEXCOORD6;
};


VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    Out.vPosition = Compute_FinalMatrix(In.vPosition);
    
    vector vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    vector vTangent = mul(float4(In.vTangent, 0.f), g_WorldMatrix);
    
    Out.vNormal = normalize(vNormal);
    Out.vTangent = normalize(vTangent);
    Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));
    Out.vTexUV0 = In.vTexUV0;
    Out.vTexUV1 = In.vTexUV1;
    Out.vTexUV2 = In.vTexUV2;
    Out.vTexUV3 = In.vTexUV3;
    Out.vTexUV4 = In.vTexUV4;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    float2 vTexUV0 : TEXCOORD0;
    float2 vTexUV1 : TEXCOORD1;
    float2 vTexUV2 : TEXCOORD2;
    float2 vTexUV3 : TEXCOORD3;
    float2 vTexUV4 : TEXCOORD4;
    float4 vWorldPos : TEXCOORD5;
    float4 vProjPos : TEXCOORD6;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vPosition : SV_Target3;
};

PS_OUT PS_Main(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    VIEWPROJINFO tMainViewProj = GetViewProjInfo();
    // Out Color
    Out.vDiffuse = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV0);
    
    if(Out.vDiffuse.a <= 0.05)
        discard;
    
    vector vNormalDesc = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV0);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.w / tMainViewProj.fCamFar, In.vProjPos.z / In.vProjPos.w, 1.f, In.vPosition.w);
    Out.vPosition = In.vWorldPos;
   
    return Out;
}

#endif // _MODEL_HLSL_