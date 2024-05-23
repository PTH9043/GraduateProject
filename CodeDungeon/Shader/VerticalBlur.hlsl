#ifndef _VERTICALBLUR_HLSL_
#define _VERTICALBLUR_HLSL_

#include "ShaderGrobalFunc.hlsli"


//struct VS_IN
//{
//    float3 vPosition : POSITION;
//    float2 vTexUV : TEXCOORD0;
//};

//struct VS_OUT
//{
//    float4 vPosition : SV_POSITION;
//    float2 vTexUV : TEXCOORD0;
//};

//VS_OUT VS_Main(VS_IN In)
//{
//    VS_OUT Out = (VS_OUT) 0;

//    Out.vPosition = Compute_FinalMatrix(In.vPosition);
//    Out.vTexUV = In.vTexUV;

//    return Out;
//}

//struct PS_In
//{
//    float4 vPosition : SV_POSITION;
//    float2 vTexUV : TEXCOORD0;
//};

//struct PS_OUT
//{
//    float4 vColor : SV_Target0;
//};

//const float TexWidth = 640;
//const float TexHeight = 540;

//static const float Weight[13] =
//{
//    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1,
//0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
//};

//static const float Total = 6.2108;

//PS_OUT PS_Main(PS_In In)
//{
//    PS_OUT Out = (PS_OUT) 0;

//    float2 originalUV = In.vTexUV;
//    float2 uv = 0;
    
//    float TexUv = 1.f / (TexHeight / 2.f);
    
//    for (int i = -6; i < 6; ++i)
//    {
//        uv = originalUV + float2(0, TexUv * i);
//        Out.vColor += Weight[6 * i] * g_Texture0.Sample(g_Sampler_Clamp, uv);
//    }
//    Out.vColor /= Total;

//    return Out;
//}


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
  
};

const float TexWidth = 640;
const float TexHeight = 540;

VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = Compute_FinalMatrix(In.vPosition);
    Out.vTexUV = In.vTexUV;

    float texelSize = 1.f / TexHeight;
    Out.texCoord1 = In.vTexUV + float2(0.0f, texelSize * -4.0f);
    Out.texCoord2 = In.vTexUV + float2(0.0f, texelSize * -3.0f);
    Out.texCoord3 = In.vTexUV + float2(0.0f, texelSize * -2.0f);
    Out.texCoord4 = In.vTexUV + float2(0.0f, texelSize * -1.0f);
    Out.texCoord5 = In.vTexUV + float2(0.0f, texelSize * 0.0f);
    Out.texCoord6 = In.vTexUV + float2(0.0f, texelSize * 1.0f);
    Out.texCoord7 = In.vTexUV + float2(0.0f, texelSize * 2.0f);
    Out.texCoord8 = In.vTexUV + float2(0.0f, texelSize * 3.0f);
    Out.texCoord9 = In.vTexUV + float2(0.0f, texelSize * 4.0f);
    
    
    
    return Out;
}

struct PS_In
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
};

struct PS_OUT
{
    float4 vColor : SV_Target0;
};

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

    float weight0, weight1, weight2, weight3, weight4;
    float normalization;
   
    
    weight0 = 1.0f;
    weight1 = 0.9f;
    weight2 = 0.55f;
    weight3 = 0.18f;
    weight4 = 0.1f;

    
    normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

    
    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;
   
    
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord1) * weight4;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord2) * weight3;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord3) * weight2;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord4) * weight1;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord5) * weight0;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord6) * weight1;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord7) * weight2;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord8) * weight3;
    Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord9) * weight4;
   
    Out.vColor.a = 1;
    return Out;
}

#endif // _VERTICALBLUR_HLSL_