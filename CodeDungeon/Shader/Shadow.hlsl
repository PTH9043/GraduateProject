#ifndef _SHADOW_HLSL_
#define _SHADOW_HLSL_

#include "ShaderGrobalFunc.hlsli"
#include "LightShaderParam.hlsli"

struct VS_IN
{
    float3 vPosition : POSITION;
   
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vclipPos : POSITION;
};


VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0.f;

  
    float4 Position = float4(In.vPosition, 1.f);

    Position = mul(Position, g_WorldMatrix);
    Position = mul(Position, g_ViewProjInfoArr[0].mViewMatrix);
    Position = mul(Position, g_ViewProjInfoArr[0].mProjMatrix);
    //Position = mul(Position, g_tLightParam.mViewMatrix);
    //Position = mul(Position, g_tLightParam.mProjMatrix);
   
    Out.vPosition = Position;
  

    return Out;
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


PS_OUT PS_Main(VS_OUT In) 
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = float4(In.vPosition.z, 0, 0, 0);
    return Out;

}



#endif // _SHADOW_HLSL_