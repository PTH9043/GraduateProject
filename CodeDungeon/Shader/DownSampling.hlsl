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



PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

  /*
     vector vDiffuseDesc = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);

    float4 HighColor = float4(0, 0, 0, 0);
    float brightness = dot(vDiffuseDesc.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    if (brightness > 0.99)
        HighColor = float4(vDiffuseDesc.rgb, 1.f);
  
    Out.vColor = HighColor;
*/
    float2 baseUV = In.vTexUV * 4.0f;

    float4 colorSum = float4(0, 0, 0, 0);
    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            float2 offsetUV = baseUV + float2(x, y) / float2(1280.0f, 1080.0f);
            float4 color = g_Texture0.Sample(g_Sampler_Normal, offsetUV);
            float brightness = dot(color.rgb, float3(0.2126f, 0.7152f, 0.0722f));
            //if (brightness > 0.99)
            //{
            //    colorSum += float4(color.rgb, 1.0);
            //}
            colorSum += float4(color.rgb, 1.0);
        }
    }

    Out.vColor = colorSum / 16.0f; // 4x4 블록의 평균값
    return Out;
}

#endif // _DOWN_SAMPLE_HLSL_
