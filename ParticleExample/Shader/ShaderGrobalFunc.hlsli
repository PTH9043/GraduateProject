#ifndef _SHADERGROBALFUNC_HLSLI_
#define _SHADERGROBALFUNC_HLSLI_


#include "ShaderParams.hlsli"

// HLSL은 16바이트 단위로 정렬을 해야하기 때문에, Padding을 넣은 것이다.
// =====================

float4 Compute_FinalMatrix(float4 vPosition)
{
	float4 Out = vPosition;

	Out = mul(Out, g_WorldMatrix);
    Out = mul(Out, g_ViewProjInfoArr[g_CamID].mViewMatrix);
    Out = mul(Out, g_ViewProjInfoArr[g_CamID].mProjMatrix);

	return Out;
}

float4 Compute_FinalMatrix(float3 vPosition)
{
	float4 Out = float4(vPosition, 1.f);

	Out = mul(Out, g_WorldMatrix);
    Out = mul(Out, g_ViewProjInfoArr[g_CamID].mViewMatrix);
    Out = mul(Out, g_ViewProjInfoArr[g_CamID].mProjMatrix);
	return Out;
}

VIEWPROJINFO GetViewProjInfo()
{
    return g_ViewProjInfoArr[g_CamID];
}



bool IsColorUp(float4 vColor, float fSize) {
	if (vColor.r >= fSize && vColor.g >= fSize && vColor.b >= fSize) {
		return true;
	}
	return false;
}

bool IsColorDown(float4 vColor, float fSize) {
	if (vColor.r <= fSize && vColor.g <= fSize && vColor.b <= fSize) {
		return true;
	}
	return false;
}

#endif // _SHADERGROBALFUNC_HLSLI_