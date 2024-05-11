#ifndef _2DFIRE_
#define _2DFIRE_

#include "ShaderGrobalFunc.hlsli"



cbuffer FireNoiseBuffer : register(b13)
{
    float fFrameTime;
    float3 fScrollSpeeds;
    float3 fScales;
    float padding;
};

cbuffer FireDistortionBuffer : register(b14)
{
    float2 fDistortion1;
    float2 fDistortion2;
    float2 fDistortion3;
    float fDistortionScale;
    float fDistortionBias;
}



struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION0;
    float2 vTexCoord : TEXCOORD0;
    float2 vTexCoords1 : TEXCOORD1;
    float2 vTexCoords2 : TEXCOORD2;
    float2 vTexCoords3 : TEXCOORD3;
};


VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0.f;
 
   
    Out.vPosition = Compute_FinalMatrix(In.vPosition);
    
    Out.vTexCoord = In.vTexCoord;
    

    // 첫 번째 스케일 및 위쪽 스크롤링 속도 값을 사용하여 첫 번째 노이즈 텍스처에 대한 텍스처 좌표를 계산합니다.
    Out.vTexCoords1 = (In.vTexCoord * fScales.x);
    Out.vTexCoords1.y = Out.vTexCoords1.y + (fFrameTime * fScrollSpeeds.x);

    // 두 번째 스케일 및 위쪽 스크롤링 속도 값을 사용하여 두 번째 노이즈 텍스처의 텍스처 좌표를 계산합니다.
    Out.vTexCoords2 = (In.vTexCoord * fScales.y);
    Out.vTexCoords2.y = Out.vTexCoords2.y + (fFrameTime * fScrollSpeeds.y);

    // 세 번째 스케일 및 위쪽 스크롤링 속도 값을 사용하여 세 번째 노이즈 텍스처의 텍스처 좌표를 계산합니다.
    Out.vTexCoords3 = (In.vTexCoord * fScales.z);
    Out.vTexCoords3.y = Out.vTexCoords3.y + (fFrameTime * fScrollSpeeds.z);


    return Out;
}


struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


float3 ToneMapping(float3 hdrColor)
{
    const float gamma = 2.2;
    float3 result = float3(1.0, 1, 1) - exp(-hdrColor * 3.f); //5.f=exposure
    result = pow(result, 1.0 / gamma);
    return result;
}

PS_OUT PS_Main(VS_OUT In)
{
    PS_OUT Out = (PS_OUT)0;

    float4 noise1;
    float4 noise2;
    float4 noise3;
    float4 finalNoise;
    float perturb;
    float2 noiseCoords;
    float4 fireColor;
    float4 alphaColor;

    // 3 개의 서로 다른 텍스처 좌표를 사용하여 동일한 노이즈 텍스처를 샘플링하여 3 개의 다른 노이즈 스케일을 얻습니다.
    noise1 = g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords1);
    noise2 = g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords2);
    noise3 = g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords3);

    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;

   
    noise1.xy = noise1.xy * fDistortion1.xy;
    noise2.xy = noise2.xy * fDistortion2.xy;
    noise3.xy = noise3.xy * fDistortion3.xy;

    finalNoise = noise1 + noise2 + noise3;

   
    perturb = ((1.0f - In.vTexCoord.y) * fDistortionScale) + fDistortionBias;

  
    noiseCoords.xy = (finalNoise.xy * perturb) + In.vTexCoord.xy;


    fireColor = g_Texture0.Sample(g_Sampler_Clamp, noiseCoords.xy);

  
    alphaColor = g_Texture2.Sample(g_Sampler_Clamp, noiseCoords.xy);

    fireColor.a = alphaColor;
        // 색상 값 정규화
    float3 normalizedColor = fireColor.rgb / max(max(fireColor.r, fireColor.g), fireColor.b);
    // 밝기 증폭
    normalizedColor *= 2.0f;
    // 색상 값 클램핑
    normalizedColor = saturate(normalizedColor);
    // 원래 범위로 되돌리기
    
   
    float4 AmpColor = float4(normalizedColor * max(max(fireColor.r, fireColor.g), fireColor.b), fireColor.a);

    float4 ToneColor = float4(ToneMapping(fireColor.xyz), fireColor.a);
    Out.vColor = AmpColor;
  
 
   // Out.vColor = fireColor;
    
    return Out;
}

#endif 