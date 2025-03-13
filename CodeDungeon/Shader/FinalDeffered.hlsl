#ifndef _DEFFERED_HLSL_
#define _DEFFERED_HLSL_

#include "ShaderGrobalFunc.hlsli"

cbuffer FogBuffer : register(b14)
{
    int IsGameStart;
    int IsFogOn;
    int IsDead;
    int IsAbility;
    int IsHit;
    float GameStartTime;
    float DieTime;
    float AbilityTime;
    float HitTime;
    float3 padding;
};


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



PS_OUT PS_Main(PS_In Input)
{
    PS_OUT Out = (PS_OUT)0;


    //float distanceFromOrigin = distance(float2(640, 540), float2(Input.vPosition.xy));
    float distanceFromOrigin = distance(float2(640, 460), float2(Input.vPosition.xy));

    float4 baseColor = g_Texture0.Sample(g_Sampler_Normal, Input.vTexUV);
    //{
    //    float4 targetColor = g_Texture8.Sample(g_Sampler_Normal, Input.vTexUV);
    //    targetColor += float4(0.15, 0.15, 0.15, 0);
    //    float4 outputColor;

    //    float initialRadius = 600.0f;
    //    float finalRadius = 850.0f;
    //    float currentRadius = initialRadius; // 2.8�� ���� �ݰ�


    //    if (distanceFromOrigin <= currentRadius)
    //    {
    //        outputColor = baseColor;
    //    }
    //    else
    //    {
    //        float distanceFactor = saturate((distanceFromOrigin - currentRadius) / (finalRadius - currentRadius));
    //        outputColor = lerp(baseColor, targetColor, distanceFactor);
    //    }

    //    baseColor = outputColor;
    //}

    if (IsGameStart)
    {
        float4 targetColor = g_Texture8.Sample(g_Sampler_Normal, Input.vTexUV);
        targetColor += float4(0.15, 0.15, 0.15, 0);
        float4 outputColor;

        float initialBlendTime = 0.3f;
        float fadeOutStartTime = 1.25f;
        float fadeOutEndTime = 1.8f;

        float initialRadius = 500.f;
        float finalRadius = 800.0f;
        float currentRadius;


        if (GameStartTime > initialBlendTime)
        {
            float normalizedBlendFactor = saturate((GameStartTime - initialBlendTime) / (fadeOutEndTime - initialBlendTime));
            currentRadius = lerp(finalRadius, initialRadius, normalizedBlendFactor);
        }
        else
        {
            currentRadius = finalRadius;
        }


        if (distanceFromOrigin <= currentRadius)
        {
            outputColor = baseColor;
        }
        else
        {
            float distanceFactor = saturate((distanceFromOrigin - currentRadius) / (finalRadius - currentRadius));
            outputColor = lerp(baseColor, targetColor, distanceFactor);
        }

        baseColor = outputColor;


    }

    float transitionProgress;
    if (IsDead)
    {
        float transitionProgress = DieTime / 1.25f;

        float4 targetColor = g_Texture6.Sample(g_Sampler_Normal, Input.vTexUV);


        float blendFactor = saturate(transitionProgress);
        Out.vColor = lerp(float4(1, 0, 0, 1), targetColor, blendFactor);

    }
    else if (IsHit)
    {

        float4 targetColor = g_Texture8.Sample(g_Sampler_Normal, Input.vTexUV);

        float4 outputColor;

        float initialBlendTime = 0.3f;
        float fadeOutStartTime = 1.25f;
        float fadeOutEndTime = 1.8f;

        float initialRadius = 0.001f;
        float finalRadius = 750.0f;
        float currentRadius;


        if (HitTime > initialBlendTime)
        {
            float normalizedBlendFactor = saturate((HitTime - initialBlendTime) / (fadeOutEndTime - initialBlendTime));
            currentRadius = lerp(finalRadius, initialRadius, normalizedBlendFactor);
        }
        else
        {
            currentRadius = finalRadius;
        }


        if (distanceFromOrigin <= currentRadius)
        {
            outputColor = targetColor;
        }
        else
        {
            float distanceFactor = saturate((distanceFromOrigin - currentRadius) / (finalRadius - currentRadius));
            outputColor = lerp(targetColor, baseColor, distanceFactor);
        }


        Out.vColor = outputColor;

    }
    else if (IsAbility)
    {
        float transitionProgress = AbilityTime / 0.5f;
        float4 targetColor = g_Texture7.Sample(g_Sampler_Normal, Input.vTexUV);
        float blendFactor = saturate(transitionProgress);
        Out.vColor = lerp(baseColor, targetColor, blendFactor);
    }
    else
    {
        Out.vColor = baseColor;

    }





    //if (Out.vColor.a == 0)
    //    discard;

    float3 vPosition = g_Texture2.Sample(g_Sampler_Normal, Input.vTexUV); //Position
    float4 vGlow = g_Texture3.Sample(g_Sampler_Normal, Input.vTexUV); //Glow


    if (IsFogOn && vGlow.a != 0.5f)//vCheckIfDrawDesc�� ����ŧ�� ����Ÿ�ٿ� ���İ� 0.5�� �� �� �ȹ޵���
    {

        float3 vViewPixelPosition = mul(float4(vPosition, 1.0f), g_ViewProjInfoArr[g_CamID].mViewMatrix);

        float4 vCameraViewPosition = mul(float4(g_ViewProjInfoArr[0].vCamPosition, 1.0f), g_ViewProjInfoArr[g_CamID].mViewMatrix);

        float dx = vViewPixelPosition.x - vCameraViewPosition.x;
        float dy = vViewPixelPosition.y - vCameraViewPosition.y;
        float dz = vViewPixelPosition.z - vCameraViewPosition.z;
        float fDistanceToCamera = sqrt(dx * dx + dy * dy + dz * dz);

        float fDistanceToCam = length(abs(vViewPixelPosition - vCameraViewPosition.xyz));
        //float camDistance = (vCameraViewPosition.z);

        float fogStart = 30.0f;
        float fogEnd = 150.0f + fogStart;

        float FogFactor = saturate((fogEnd - fDistanceToCam) / (fogEnd - fogStart));
        float FogFactor2 = 1 / pow(2.781828, (fDistanceToCam * 0.0015) * (fDistanceToCam * 0.0015));


        if (vGlow.a != 1.f)
        {
            Out.vColor = lerp(float4(0.21f, 0.21f, 0.21f, 1.0f), Out.vColor, FogFactor);
        }

        //guardó�� �ڿ� �Ȱ� ���� �ޱ� �ϵ���. 
               //Glow�� �ֵ��� ���İ��� 1�� ����Ͽ� �Ȱ��� ���޵���.


    }
//
//    if (IsFogOn && vGlow.a != 0.5f) // ���� ���� �ʴ� ����Ÿ�� ����
//    {
//        // �� �������� �ȼ� ��ġ
//        float3 vViewPixelPosition = mul(float4(vPosition, 1.0f), g_ViewProjInfoArr[g_CamID].mViewMatrix).xyz;
//
//        // ī�޶� ��ġ (�� ����)
//        float vCameraDepth = g_ViewProjInfoArr[0].vCamPosition.z;
//
//        // �ȼ��� ī�޶� ���� �Ÿ�
//        float fDistanceToCamera = abs(vViewPixelPosition.z - vCameraDepth);
//
//        // Fog ���
//        float fogStart = 30.0f;
//        float fogEnd = 180.0f; // (150 + 30)
//        float FogFactor = saturate((fogEnd - fDistanceToCamera) / max(0.001f, fogEnd - fogStart));
//        float FogFactor2 = exp(-pow(fDistanceToCamera * 0.0015, 2));
//
//        // �Ȱ� ���� (Glow�� 1�̸� �Ȱ� ���� ���� ����)
//    Out.vColor = (vGlow.a == 1.0f)
//        ? Out.vColor
//        : lerp(float4(0.21f, 0.21f, 0.21f, 1.0f), Out.vColor, FogFactor);
//}
//    
  
    Out.vColor += g_Texture1.Sample(g_Sampler_Normal, Input.vTexUV); //AlphaDeffered
    float vDepthDesc = g_Texture4.Sample(g_Sampler_Normal, Input.vTexUV);
    if (IsAbility)
    {
        float4 outline = g_Texture9.Sample(g_Sampler_Normal, Input.vTexUV);
    
       
   
        if (vDepthDesc == 1.f && outline.w > 0.f)
        {
            Out.vColor.xyz += outline.xyz;
        }
    }
    else
    {        
        float4 outline = g_Texture5.Sample(g_Sampler_Normal, Input.vTexUV);
    
        
        if ((vDepthDesc == 1.f && outline.w >0.f))
        {
            Out.vColor.xyz += outline.xyz;
        }
    }
   
   
  
    
    return Out;
}


#endif // _DEFFERED_HLSL_