#ifndef _BLOOM_HLSL_
#define _BLOOM_HLSL_

#include "ShaderGrobalFunc.hlsli"


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

    Out.vPosition = Compute_DefferedFinalMatrix(In.vPosition);
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
    float4 vColor : SV_Target0;
};



//PS_OUT PS_Main(PS_In In)
//{
//    PS_OUT Out = (PS_OUT) 0;

//    vector vHDRColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
//    vector vBlurColor = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV);
//    vector vOriginalColor = g_Texture2.Sample(g_Sampler_Normal, In.vTexUV);
        
    
//    float4 vBloom = pow(pow(abs(vBlurColor), 2.2f) + pow(abs(vOriginalColor), 2.2f), 1.f / 2.2f);
  
//    float4 OutColor = vHDRColor;
    
//    OutColor = pow(abs(OutColor), 2.2f);
//    vBloom = pow(abs(vBloom), 2.2f);
//    OutColor += vBloom;
//    Out.vColor = pow(abs(OutColor), 1 / 2.2f);
    

//    return Out;
//}

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vHDRColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
    vector vBlurColor = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV);
    vector vOriginalColor = g_Texture2.Sample(g_Sampler_Normal, In.vTexUV);

    // �� ȿ���� �����ϱ� ���� �� �ؽ�ó�� �⿩���� ���Դϴ�.
    float blurWeight = 2.0f; // �� �⿩�� ����ġ
    vector enhancedBlurColor = vBlurColor * blurWeight;

    float4 vBloom = pow(pow(abs(enhancedBlurColor), 2.2f) + pow(abs(vOriginalColor), 2.2f), 1.f / 2.2f);
  
    float4 OutColor = vHDRColor;
    
    OutColor = pow(abs(OutColor), 2.2f);
    vBloom = pow(abs(vBloom), 2.2f);
    OutColor += vBloom;
    Out.vColor = pow(abs(OutColor), 1 / 2.2f);
    
    return Out;
}

//PS_OUT PS_Main(PS_In In)
//{
//    PS_OUT Out = (PS_OUT) 0;

//    vector vHDRColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
//    vector vBlurColor = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV);
//    vector vOriginalColor = g_Texture2.Sample(g_Sampler_Normal, In.vTexUV);

//    // ���� ���� �����մϴ�.
//    float gamma = 2.2f;
//    float blurGamma = 2.4f; // �� ȿ���� �����ϱ� ���� �� �ؽ�ó�� �ٸ� ���� ���� ����մϴ�.

//    float4 vBloom = pow(pow(abs(vBlurColor), blurGamma) + pow(abs(vOriginalColor), gamma), 1.f / gamma);
  
//    float4 OutColor = vHDRColor;
    
//    OutColor = pow(abs(OutColor), gamma);
//    vBloom = pow(abs(vBloom), gamma);
//    OutColor += vBloom;
//    Out.vColor = pow(abs(OutColor), 1 / gamma);
    
//    return Out;
//}

//PS_OUT PS_Main(PS_In In)
//{
//    PS_OUT Out = (PS_OUT) 0;

//    vector vHDRColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
//    vector vBlurColor = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV);
//    vector vOriginalColor = g_Texture2.Sample(g_Sampler_Normal, In.vTexUV);

//    // �� �ؽ�ó�� ���� �ؽ�ó�� �⿩���� �����Ͽ� �� ȿ���� �����մϴ�.
//    float originalWeight = 0.5f; // ���� �ؽ�ó ����ġ
//    float blurWeight = 1.5f; // �� �ؽ�ó ����ġ

//    float4 vBloom = pow(pow(abs(vBlurColor * blurWeight), 2.2f) + pow(abs(vOriginalColor * originalWeight), 2.2f), 1.f / 2.2f);
  
//    float4 OutColor = vHDRColor;
    
//    OutColor = pow(abs(OutColor), 2.2f);
//    vBloom = pow(abs(vBloom), 2.2f);
//    OutColor += vBloom;
//    Out.vColor = pow(abs(OutColor), 1 / 2.2f);
    
//    return Out;
//}

#endif // _BLOOM_HLSL_