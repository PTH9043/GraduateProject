#ifndef _LIGHTSHADERPARAM_HLSLI_
#define _LIGHTSHADERPARAM_HLSLI_

struct LIGHTINFO
{
    float4      vDiffuse;
    float4      vAmbient;
    float4      vSpecular;
    float4      vDirection;
    float4      vPosition;
    
    int            eLightType;
    float         fRange;
    float         fAngle;
    float         fLightPower;
    
    float         fSpecularPowValue;
    float3       vPadding;
};

struct LIGHTCOLOR
{
    float4 vDiffuse;
    float4 vAmbient;
    float4 vSpecular;
};

struct LIGHTCONTROL
{
     // Single Light Cam Index
    float fShadowDepth;
    float fRimLightPower;
    float fRimMin;
    float fRimMax;
    
    float3 vAttenent;
    float fPadding;
    
    float4 vShadowColor;
};

struct LIGHTPARAM
{
    float3       vLightCamPos;
    float          fCamFar;
    matrix       mScreenWorldMatrix;
    matrix       mScreenViewMatrix;
    matrix       mScreenProjMatrix;
    matrix       mViewMatrix;
    matrix       mProjMatrix;
    matrix       mViewMatrixInv;
    matrix       mProjMatrixInv;
};

cbuffer LIGHTBUFFER : register(b2)
{
    LIGHTPARAM g_tLightParam;
};


cbuffer LIGHTCONTROLPARAM : register(b4)
{
    LIGHTINFO                       g_tLightInfo;
    LIGHTCONTROL              g_tLightControl;
};

float4 g_vMaterialSpecular = float4(1.f, 1.f, 1.f, 1.f);
float4 g_vMaterialAmbient = float4(1.f, 1.f, 1.f, 1.f);


SamplerState g_Sampler_Normal : register(s0);
SamplerState g_Sampler_Clamp : register(s1);

// 14 ~ 15 3DTexture
Texture2D g_Texture0 : register(t0);
Texture2D g_Texture1 : register(t1);
Texture2D g_Texture2 : register(t2);
Texture2D g_Texture3 : register(t3);
Texture2D g_Texture4 : register(t4);


#endif // _LIGHTSHADERPARAM_HLSLI_
