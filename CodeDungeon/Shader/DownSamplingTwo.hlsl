#ifndef _DOWN_SAMPLE_HLSL_
#define _DOWN_SAMPLE_HLSL_

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
    Out.vPosition = Compute_FinalMatrix(In.vPosition);
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

const float2 inverseTextureSize = float2(1.0 / 1280.0, 1.0 / 1080.0); // 입력 텍스처의 해상도에 대한 역수

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

    // UV 좌표를 6x6 블록의 시작 위치로 변환
    float2 baseUV = In.vTexUV * 24.0f;

    float4 colorSum = float4(0, 0, 0, 0);
    for (int y = 0; y < 6; ++y)
    {
        for (int x = 0; x < 6; ++x)
        {
            float2 offsetUV = baseUV + float2(x, y) / float2(1280.0f / 4.f, 1080.0f / 4.f);
            float4 color = g_Texture0.Sample(g_Sampler_Normal, offsetUV);
            colorSum += color;
        }
    }

    Out.vColor = colorSum / 36.0f; // 6x6 블록의 평균값
    return Out;
}

#endif // _DOWN_SAMPLE_HLSL_
