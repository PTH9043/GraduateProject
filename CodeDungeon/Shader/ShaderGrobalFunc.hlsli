#ifndef _SHADERGROBALFUNC_HLSLI_
#define _SHADERGROBALFUNC_HLSLI_


#include "ShaderParams.hlsli"

// HLSL�� 16����Ʈ ������ ������ �ؾ��ϱ� ������, Padding�� ���� ���̴�.
// =====================

float4 Compute_DefferedFinalMatrix(float4 vPosition)
{
    float4 Out = vPosition;

    Out = mul(Out, deffered_WorldMatrix);
    Out = mul(Out, g_ViewProjInfoArr[deffered_CamID].mViewMatrix);
    Out = mul(Out, g_ViewProjInfoArr[deffered_CamID].mProjMatrix);

    return Out;
}

float4 Compute_DefferedFinalMatrix(float3 vPosition)
{
    float4 Out = float4(vPosition, 1.f);

    Out = mul(Out, deffered_WorldMatrix);
    Out = mul(Out, g_ViewProjInfoArr[deffered_CamID].mViewMatrix);
    Out = mul(Out, g_ViewProjInfoArr[deffered_CamID].mProjMatrix);
    return Out;
}

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

float4 Compute_FinalMatrix_Socket(float4 vPosition, float4x4 _SocketMatrix)
{
    float4 Out = vPosition;
    
    matrix matWS, matWSV, matWSVP;
    matWS = mul(g_WorldMatrix, _SocketMatrix);
    matWSV = mul(matWS, g_ViewProjInfoArr[g_CamID].mViewMatrix);
    matWSVP = mul(matWSV, g_ViewProjInfoArr[g_CamID].mProjMatrix);

    Out = mul(Out, matWSVP);
    return Out;
}

float4 Compute_FinalMatrix_Socket(float3 vPosition, float4x4 _SocketMatrix)
{
    float4 Out = float4(vPosition, 1.f);

    matrix matWS, matWSV, matWSVP;
    matWS = mul(g_WorldMatrix, _SocketMatrix);
    matWSV = mul(matWS, g_ViewProjInfoArr[g_CamID].mViewMatrix);
    matWSVP = mul(matWSV, g_ViewProjInfoArr[g_CamID].mProjMatrix);

    Out = mul(Out, matWSVP);
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

float CalculateTessLevel(float3 cameraWorldPos, float3 patchPos, float min, float max, float maxLv)
{
    float distance = length(patchPos - cameraWorldPos);

    if (distance < min)
        return maxLv;
    if (distance > max)
        return 1.f;

    float ratio = (distance - min) / (max - min);
    float level = (maxLv - 1.f) * (1.f - ratio);
    return level;
}

#endif // _SHADERGROBALFUNC_HLSLI_