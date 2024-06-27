#ifndef _DEFFERED_HLSL_
#define _DEFFERED_HLSL_

#include "ShaderGrobalFunc.hlsli"

cbuffer FogBuffer : register(b14)
{
    bool IsFogOn;
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

PS_OUT PS_Main(PS_In Input)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture0.Sample(g_Sampler_Normal, Input.vTexUV); //BlendDeffered 결과물
    //if (Out.vColor.a == 0)
    //    discard;
    
   
   
    if (IsFogOn)
    {
        float3 vPosition = g_Texture2.Sample(g_Sampler_Normal, Input.vTexUV); //Position
        float4 vGlow = g_Texture3.Sample(g_Sampler_Normal, Input.vTexUV); //Glow
        float3 vViewPixelPosition = mul(float4(vPosition, 1.0f), g_ViewProjInfoArr[g_CamID].mViewMatrix);

        float4 vCameraViewPosition = mul(float4(g_ViewProjInfoArr[0].vCamPosition, 1.0f), g_ViewProjInfoArr[g_CamID].mViewMatrix);
  
        float dx = vViewPixelPosition.x - vCameraViewPosition.x;
        float dy = vViewPixelPosition.y - vCameraViewPosition.y;
        float dz = vViewPixelPosition.z - vCameraViewPosition.z;
        float fDistanceToCamera = sqrt(dx * dx + dy * dy + dz * dz);
   
        //float camDistance = (vCameraViewPosition.z);
        float fogStart = 30.0f;
        float fogEnd = 150.0f + fogStart;
   
        float FogFactor = saturate((fogEnd - fDistanceToCamera) / (fogEnd - fogStart));
        float FogFactor2 = 1 / pow(2.781828, (fDistanceToCamera * 0.0015) * (fDistanceToCamera * 0.0015));
        
        if (vGlow.a != 0.5f)//trail을 0.5로 하여 안개 안받도록
        {
            if (vGlow.a != 1.f)
            {
                Out.vColor = lerp(float4(0.21f, 0.21f, 0.21f, 1.0f), Out.vColor, FogFactor);
            }
            else
            {
                if (fDistanceToCamera > 60)//이면 
                    Out.vColor = lerp(float4(0.2f, 0.15f, 0.04f, 1.f), Out.vColor, FogFactor);
                else
                    Out.vColor = lerp(float4(0.2f, 0.15f, 0.04f, 1.f), Out.vColor, FogFactor2);
            //Out.vColor = lerp(float4(0.2f, 0.15f, 0.04f, 1.f), Out.vColor, FogFactor2);
            }
               //Glow인 애들은 알파값이 1로 기록하여 안개를 덜받도록.
        }
        else
        {
            Out.vColor = lerp(vGlow, Out.vColor, FogFactor);
        }
      
     }
 
    Out.vColor += g_Texture1.Sample(g_Sampler_Normal, Input.vTexUV); //AlphaDeffered

    float4 outline = g_Texture5.Sample(g_Sampler_Normal, Input.vTexUV);
    
    float4 vDepthDesc = g_Texture4.Sample(g_Sampler_Normal, Input.vTexUV);
    
    if (vDepthDesc.g >= 1.f && outline.w <= 1.f)
    {
        Out.vColor.xyz += outline.xyz;
    }
  
    
    return Out;
}


#endif // _DEFFERED_HLSL_