#ifndef _LIGHTSHADERPARAM_HLSLI_
#define _LIGHTSHADERPARAM_HLSLI_

#include "ShaderParams.hlsli"

struct LIGHTINFO
{
    float4      vDiffuse;
    float4      vAmbient;
    float4      vSpecular;
    float4      vDirection;
    float4      vPosition;
    
    int            eLightType;
    int         eLightVersion;
    float         fRange;
    float         fAngle;
    float         fLightPower;
    
    float         fSpecularPowValue;
    float fFallOff;
    float fPhi;
    float fTheta;
    float3 vAttenuation;
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

cbuffer LIGHTARRAYPARAM : register(b4)
{
    
}

float4 g_vMaterialSpecular = float4(1.f, 1.f, 1.f, 1.f);
float4 g_vMaterialAmbient = float4(1.f, 1.f, 1.f, 1.f);
float g_vGlobalAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);


#endif // _LIGHTSHADERPARAM_HLSLI_
