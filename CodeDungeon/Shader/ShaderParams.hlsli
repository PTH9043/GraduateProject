#ifndef _SHADERPARAM_HLSLI_
#define _SHADERPARAM_HLSLI_

// HLSL은 16바이트 단위로 정렬을 해야하기 때문에, Padding을 넣은 것이다.
// =====================

struct VIEWPROJINFO
{
    matrix    mViewMatrix;
    matrix    mProjMatrix;
    matrix    mPrevViewMatirx;
    // Cam 
    float       fCamFar;
    float3     vCamPosition;
};
    // 태현 추가
struct MODELMATERIALINFO
{
    float4 vDiffuse;
    float4 vAmbient;
    float4 vSpecular;
    float4 vEmissive;
    float4 vTransparent;
    float4 vReflective;
    float fShininess;
    float fOpacity;
    float fTransparencyFactor;
    float fBumpScaling;
    float fReflectivity;
};

    // 태현 추가
struct MATERIALGROBALINFO
{
		// Model MaterialInfo
    MODELMATERIALINFO stModelMaterialInfoGroup[1000];
};

cbuffer VIEWPROJ_PARAMS : register(b0)
{
    VIEWPROJINFO g_ViewProjInfoArr[10];
    bool g_isObjectMotionBlur;
    float fGrobalDeltaTime;
    bool g_ViewProjBoolPadding1;
    bool g_ViewProjBoolPadding2;
    bool g_ViewProjBoolPadding3;
    // Padding Value
    float2 g_ViewProjPaddings;
};
    // 태현 추가
cbuffer GROBALINFO_PARAMS : register(b1)
{
    MATERIALGROBALINFO g_MaterialGrobalInfo;
}

cbuffer TRANSFORM_PARAMS : register(b3)
{
    matrix     g_WorldMatrix;
    matrix     g_PrevWorldMatrix;
    int            g_CamID;
    float3      g_TransformPadding;
};

SamplerState g_Sampler_Normal : register(s0);
SamplerState g_Sampler_Clamp : register(s1);

// 14 ~ 15 3DTexture
Texture2D g_Texture0 : register(t0);
Texture2D g_Texture1 : register(t1);
Texture2D g_Texture2 : register(t2);
Texture2D g_Texture3 : register(t3);
Texture2D g_Texture4 : register(t4);
Texture2D g_Texture5 : register(t5);
Texture2D g_Texture6 : register(t6);
Texture2D g_Texture7 : register(t7);
Texture2D g_Texture8 : register(t8);
TextureCube g_Texture9 : register(t9);

int MAIN_CAM_ID = 0;
int DEFFERED_CAM_ID = 1;
int SHADOWLIGHT_CAM_ID = 2;
 
#endif // _SHADERPARAM_HLSLI_