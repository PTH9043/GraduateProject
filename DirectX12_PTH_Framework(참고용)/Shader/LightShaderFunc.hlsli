#ifndef _LIGHTSHADERFUNC_HLSLI_
#define _LIGHTSHADERFUNC_HLSLI_

#include "LightShaderParam.hlsli"

// 시야 각도를 계산하는 함수
float diffAngle(float3 dir1, float3 dir2)
{
    return acos(dot(dir1, dir2));
}

float AttenuationRatio(float distance)
{
    return 1.0f / (g_tLightControl.vAttenent.x + g_tLightControl.vAttenent.y * distance + g_tLightControl.vAttenent.z * distance * distance);
}

float AttenuationRangeRatio(float distance)
{
    float attenuation = AttenuationRatio(distance);
    float rangeAttenuation = saturate(1.0f - distance / g_tLightInfo.fRange);
    return attenuation * rangeAttenuation;

}

void JudgeShading(in float4 vNormal, out float fLightDensity)
{
    fLightDensity = pow((fLightDensity * 0.5f) + 0.5f, 2); // 하프 램버트
	// 캐릭터한테만 카툰 쉐이딩
    if (1.f == vNormal.a)
    {
        fLightDensity = fLightDensity * 0.5f + 0.5f;
        fLightDensity = floor(fLightDensity * 2.f) * (1 / 2.f);
    }
}

void DirectionShade(in float4 vNormalDesc, out float4 vShade)
{
    float4 vNormal = vNormalDesc;
    float4 vLightDir = normalize(g_tLightInfo.vDirection);
    vLightDir.w = 0.f;
    float fLightDensity = saturate(dot(vLightDir * -1.f, vNormal)) * g_tLightInfo.fLightPower;
    
    // 캐릭터 카툰 쉐이딩
    // JudgeShading(vNormalDesc, fLightDensity);

    vShade = g_tLightInfo.vDiffuse * fLightDensity;
    vShade.a = 1.f;
}

void DirectionalSpecular(in float4 vNormal, in float4 vWorldPos, out float4 vSepecular)
{
    float4 vCamLook = normalize(float4(vWorldPos.xyz - g_tLightParam.vLightCamPos.xyz, 0.f));
    float4 vLightDir = normalize(g_tLightInfo.vDirection);
    vLightDir.w = 0.f;
    float4 reflection = reflect(-vLightDir, vNormal);
    float fLightDensity = pow(dot(vCamLook, reflection), g_tLightInfo.fSpecularPowValue);
        
    vSepecular = (g_tLightInfo.vSpecular * fLightDensity);
    vSepecular.a = 1.f;
}

void DirectionalShadow(in float4 vWorldPos, out float4 vShadow)
{
    vector vLightProjPos = mul(vWorldPos, g_tLightParam.mViewMatrix);
    vLightProjPos = mul(vLightProjPos, g_tLightParam.mProjMatrix);
	// currentDepth : 화면에 나오는 픽셀의 Light 위치의 View 깊이
    float currentDepth = vLightProjPos.w;
    float2 uv = vLightProjPos.xy / vLightProjPos.w;
    uv.x = uv.x * 0.5f + 0.5f;
    uv.y = uv.y * -0.5f + 0.5f;
    vShadow = vector(1.f, 1.f, 1.f, 1.f);
    if (0.f <= uv.x && uv.x <= 1.f && 0.f < uv.y && uv.y < 1.f)
    {
        // Dpeth Info
        vector vLightDepth = g_Texture1.Sample(g_Sampler_Clamp, uv);
        float lightDepth = vLightDepth.r * g_tLightParam.fCamFar;
        if (currentDepth - g_tLightControl.fShadowDepth > lightDepth)
        {
            vShadow = g_tLightControl.vShadowColor;
        }
    }
}

float RimLight(in float4 vWorldPos, in float4 vNormal)
{
    float4 vCamLook = normalize(float4(vWorldPos.xyz - g_tLightParam.vLightCamPos.xyz, 0.f));
    float4 vLightDir = normalize(g_tLightInfo.vDirection);
    vLightDir.w = 0.f;
    float fLightDensity = dot(vCamLook * -1.f, normalize(vNormal));

    float rimLightIntensity = max(0.f, 1.f - fLightDensity);
    rimLightIntensity = pow(rimLightIntensity, g_tLightControl.fRimLightPower);
    rimLightIntensity = smoothstep(g_tLightControl.fRimMin, g_tLightControl.fRimMax, rimLightIntensity);
    if (0.f == g_tLightControl.fRimLightPower)
        rimLightIntensity = 0.f;

    return rimLightIntensity;
}

void PointShade(in float4 vNormalDesc, in float4 vWorldPos, out float4 vShade)
{
    float4 vNormal = vNormalDesc;
    float4 vLightDir = vWorldPos - g_tLightInfo.vPosition;
    vLightDir.w = 0.f;
    
    float fDistance = length(vLightDir);
    float fLightDensity = dot(normalize(vLightDir) * -1.f, vNormal);
    float fAtt = 0.f;
    if (g_tLightInfo.fRange != 0.f)
    {
        fAtt = saturate((g_tLightInfo.fRange - fDistance) / g_tLightInfo.fRange);
    }
    // 캐릭터 카툰 쉐이딩
    // JudgeShading(vNormalDesc, fLightDensity);
    
    vShade = g_tLightInfo.vDiffuse * fLightDensity * fAtt;
    vShade.a = 1.f;
}

void PointSpecular(in float4 vNormalDesc, in float4 vWorldPos, in float fPower, out float4 vSpecular)
{
    float4 vReflect = normalize(reflect(normalize(g_tLightInfo.vDirection), vNormalDesc));
    float4 vCamLook = normalize(float4(vWorldPos.xyz - g_tLightParam.vLightCamPos.xyz, 0.f));
    float4 vLook = vWorldPos - float4(g_tLightParam.vLightCamPos, 1.f);
    
    vSpecular = g_tLightInfo.vSpecular * g_vMaterialSpecular * pow(saturate(dot(normalize(vLook * -1.f), vReflect)), fPower);
    vSpecular.a = 1.f;
}

LIGHTCOLOR CalculateLightColor(float3 vViewNormal, float3 vViewPos, float3 vWorldNormal, float3 vWorldPos)
{
    LIGHTCOLOR color = (LIGHTCOLOR) 0.f;

    float3 viewLightDir = (float3) 0.f;

    float diffuseRatio = 0.f;
    float specularRatio = 0.f;
    float distanceRatio = 1.f;
    float angleFactor = 1.f;

    if (g_tLightInfo.eLightType == 0)
    {
        // Directional Light
        viewLightDir = normalize(mul(float4(g_tLightInfo.vDirection.xyz, 0.f), g_tLightParam.mViewMatrix).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, vViewNormal));
    }
    else if (g_tLightInfo.eLightType == 1)
    {
        // Spot Light
        float3 viewLightPos = mul(float4(g_tLightInfo.vPosition.xyz, 1.f), g_tLightParam.mViewMatrix).xyz;
        float3 viewLightVec = vViewPos - viewLightPos;
        viewLightDir = normalize(viewLightVec);
        diffuseRatio = saturate(dot(-viewLightDir, vViewNormal));

        float dist = distance(vViewPos, viewLightPos);
        if (g_tLightInfo.fRange == 0.f)
            distanceRatio = 0.f;
        else
        {
            distanceRatio = saturate(pow(AttenuationRangeRatio(dist), 2));
        }
    }
    else if (g_tLightInfo.eLightType == 2)
    {
        // Spot Light
        float3 viewLightPos = mul(float4(g_tLightInfo.vPosition.xyz, 1.f), g_tLightParam.mViewMatrix).xyz;
        float3 viewLightVec = vViewPos - viewLightPos;
        viewLightDir = normalize(viewLightVec);
        diffuseRatio = saturate(dot(-viewLightDir, vViewNormal));

        float spotAngle = radians(g_tLightInfo.fAngle);
        
        if (g_tLightInfo.fRange == 0.f)
            distanceRatio = 0.f;
        else
        {
            float halfAngle = (spotAngle) / 2;

            float3 viewLightVec = vViewPos - viewLightPos;
            float3 viewCenterLightDir = normalize(mul(float4(g_tLightInfo.vDirection.xyz, 0.f), g_tLightParam.mViewMatrix).xyz);

            float centerDist = dot(viewLightVec, viewCenterLightDir);

            float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));
            if (centerDist < 0.f || centerDist > g_tLightInfo.fRange) // 최대 거리를 벗어났는지
                distanceRatio = 0.f;
            else if (lightAngle > spotAngle) // 20도를 넘어가면 빛이 더 이상 퍼지지 않음
                distanceRatio = 0.f;
            else
            {
                float softFactor = (1 - (lightAngle / halfAngle));
                distanceRatio = saturate(softFactor) * AttenuationRangeRatio(length(viewLightVec));
            }
        }
    }
    else if (g_tLightInfo.eLightType == 3)
    {
        float3 viewLightPos = mul(float4(g_tLightInfo.vPosition.xyz, 1.f), g_tLightParam.mViewMatrix).xyz;
        float3 viewLightVec = vViewPos - viewLightPos;
        viewLightDir = normalize(viewLightVec);
        diffuseRatio = saturate(dot(-viewLightDir, vViewNormal));

        float3 viewDirection = normalize(mul(float4(g_tLightInfo.vDirection.xyz, 0.f), g_tLightParam.mViewMatrix).xyz);

        // Calculate the angle between the light direction and the view direction
        float angle = acos(dot(viewLightDir, viewDirection));

        float maxAngle = radians(g_tLightInfo.fAngle); // 최대 각도 (라디안으로 변환)
        float centerDist = length(viewLightVec); // 빛의 위치와 뷰 위치 간의 거리
        
        // 빛의 위치가 퍼져야 하는 거리 내에 있으면 빛을 퍼뜨림
        float intensity = 1.0 - angle / maxAngle; // 최대 각도에 따라 감쇠된 빛의 강도 계산
        distanceRatio = saturate(intensity * AttenuationRangeRatio(centerDist)); // 빛의 강도에 거리와 감쇠된 빛의 강도를 곱해줌
    }

    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, vViewNormal)) * vViewNormal));
    float3 eyeDir = normalize(vViewPos);
    specularRatio = saturate(dot(-eyeDir, reflectionDir));
    specularRatio = saturate(pow(specularRatio, g_tLightInfo.fSpecularPowValue));
    
    color.vDiffuse = g_tLightInfo.vDiffuse * diffuseRatio * distanceRatio;
    color.vAmbient = g_tLightInfo.vAmbient * distanceRatio;
    color.vSpecular = g_tLightInfo.vSpecular * specularRatio * distanceRatio;
    
    color.vDiffuse = saturate(color.vDiffuse * g_tLightInfo.fLightPower);
    
    
    color.vDiffuse.a = 1.f;
    return color;
}



#endif // _LIGHTSHADERFUNC_HLSLI_