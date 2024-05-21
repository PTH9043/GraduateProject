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

    float4 downScaled = float4(0, 0, 0, 0);

    // 네 개의 픽셀을 하나의 픽셀로 합칩니다.
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            // 다운샘플링된 좌표 계산
            float2 downSampledTexCoord = In.vTexUV * 2.0f + float2(i, j);

            // 샘플링하여 픽셀 값을 합산
            downScaled += g_Texture0.Sample(g_Sampler_Normal, downSampledTexCoord);
        }
    }

    // 네 개의 픽셀 값을 평균하여 다운샘플링된 픽셀 값을 계산
    downScaled /= 4.0f;

    // 계산된 다운샘플링된 픽셀 값을 출력에 설정합니다.
    Out.vColor = downScaled;

    return Out;
}

#endif // _DOWN_SAMPLE_HLSL_
