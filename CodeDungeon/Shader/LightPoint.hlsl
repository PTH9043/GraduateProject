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
//해상도 1280 1080
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

    //월드에서는 vNormal을 텍스쳐로부터 읽고 그대로 쓰는게 조명이 확실히 DiffuseFactor계산이 됨.
    // 뷰공간에선 노멀라이즈하는게 좋아보임.
    // 
    //// 0, 1, 3
    float4 vNormal = g_Texture0.Sample(g_Sampler_Normal, Input.vTexUV);
   //vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 0.f));
    float4 vDepth = g_Texture1.Sample(g_Sampler_Normal, Input.vTexUV);
    float4 vPosition = g_Texture2.Sample(g_Sampler_Normal, Input.vTexUV);
    vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 0.f));
    float4 vViewPosition = mul(vPosition, g_tLightParam.mViewMatrix);
    float4 vViewNormal = mul(vNormal, g_tLightParam.mViewMatrix);
    vViewNormal = normalize(vViewNormal);
  

    LIGHTCOLOR tLightColor = (LIGHTCOLOR) 0.f;
        // 태현 추가
   // MODELMATERIALINFO ModelMaterialDataInfo = g_MaterialGrobalInfo.stModelMaterialInfoGroup[vDepth.z];
    
    if (g_tLightInfo.eLightVersion == 0)
    {
        
        tLightColor = CalculateLightColorInWorldSpace(vNormal.xyz, vPosition.xyz); //태현이꺼
       
        Out.vAmbient = tLightColor.vAmbient;
        Out.vShade = tLightColor.vDiffuse;
        Out.vSpecular = tLightColor.vSpecular;
    }
    else if(g_tLightInfo.eLightVersion == 1)
    {
     
       // tLightColor = LightingInView(vViewPosition.xyz, vViewNormal.xyz);
        tLightColor = LightingInWorld(vPosition.xyz, vNormal.xyz); //용빠 월드

        Out.vAmbient = tLightColor.vAmbient;
        Out.vShade = tLightColor.vDiffuse;
        Out.vSpecular = tLightColor.vSpecular;
    }else{

        tLightColor = LightingInWorld2(vPosition.xyz, vNormal.xyz); //테스트용 빛 : 감쇠적용x specular전역
        Out.vAmbient = tLightColor.vAmbient;
        Out.vShade = tLightColor.vDiffuse;
        Out.vSpecular = tLightColor.vSpecular;
    }


  
	return Out;
}

#endif // _LIGHTDEFFERED_HLSL_