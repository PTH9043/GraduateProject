#ifndef _SHADOW_HLSL_
#define _SHADOW_HLSL_

#include "ShaderGrobalFunc.hlsli"

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

  
    Out.vPosition = Compute_FinalMatrix(Out.vPosition);
    Out.vclipPos = Out.vPosition;

    return Out;
}

float4 PS_Main(VS_OUT In) : SV_Target
{
    return float4(In.vclipPos.z / In.vclipPos.w, 0.f, 0.f, 0.f);
}



#endif // _SHADOW_HLSL_