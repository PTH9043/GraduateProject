#ifndef _TESSELLATION_FX_
#define _TESSELLATION_FX_

#include "ShaderGrobalFunc.hlsli"

cbuffer TERRAINTESSINFO : register(b13)
{
    int2        g_Tiles;
    float2     g_HeightMapResolution;
    float2     g_MinMaxTessellationDistance;
    float        g_TesselationMaxLevel;
    float       g_TessPadding;
};

// --------------
// Vertex Shader
// --------------

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    Out.vPosition = In.vPosition;
    Out.vNormal = In.vNormal;
    Out.vTexUV = In.vTexUV;
    return Out;
}

// --------------
// Hull Shader
// --------------

struct PatchTess
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};

struct HS_OUT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vWorldPos : TEXCOORD1;
};

float CalculateTessFactor(float3 f3Position)
{
    float fDistToCamera = distance(f3Position, g_ViewProjInfoArr[g_CamID].vCamPosition);
    float s = saturate((fDistToCamera - 10.0f) / (500.0f - 10.0f));

    return (lerp(64.0f, 1.0f, s));
}

// Control Point HS
[domain("tri")] // 패치의 종류 (tri, quad, isoline)
[partitioning("fractional_odd")] // subdivision mode (integer 소수점 무시, fractional_even, fractional_odd)
[outputtopology("triangle_cw")] // (triangle_cw, triangle_ccw, line)
[outputcontrolpoints(3)] // 하나의 입력 패치에 대해, HS가 출력할 제어점 개수
[patchconstantfunc("ConstantHS")] // ConstantHS 함수 이름
HS_OUT HS_Main(InputPatch<VS_OUT, 3> input, int vertexIdx : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;

    output.vPosition = input[vertexIdx].vPosition;
    output.vTexUV = input[vertexIdx].vTexUV;
    output.vNormal = input[vertexIdx].vNormal;
    return output;
}

// Constant HS
PatchTess ConstantHS(InputPatch<VS_OUT, 3> input, int patchID : SV_PrimitiveID)
{
    PatchTess output = (PatchTess) 0.f;

    float minDistance = g_MinMaxTessellationDistance.x;
    float maxDistance = g_MinMaxTessellationDistance.y;

    float3 edge0Pos = (input[1].vPosition + input[2].vPosition) / 2.f;
    float3 edge1Pos = (input[2].vPosition + input[0].vPosition) / 2.f;
    float3 edge2Pos = (input[0].vPosition + input[1].vPosition) / 2.f;

    edge0Pos = mul(float4(edge0Pos, 1.f), g_WorldMatrix).xyz;
    edge1Pos = mul(float4(edge1Pos, 1.f), g_WorldMatrix).xyz;
    edge2Pos = mul(float4(edge2Pos, 1.f), g_WorldMatrix).xyz;

    float edge0TessLevel = CalculateTessLevel(g_ViewProjInfoArr[g_CamID].vCamPosition.xyz, edge0Pos, minDistance, maxDistance, 4.f);
    float edge1TessLevel = CalculateTessLevel(g_ViewProjInfoArr[g_CamID].vCamPosition.xyz, edge1Pos, minDistance, maxDistance, 4.f);
    float edge2TessLevel = CalculateTessLevel(g_ViewProjInfoArr[g_CamID].vCamPosition.xyz, edge2Pos, minDistance, maxDistance, 4.f);

    output.edgeTess[0] = edge0TessLevel;
    output.edgeTess[1] = edge1TessLevel;
    output.edgeTess[2] = edge2TessLevel;
    output.insideTess = edge2TessLevel;

    return output;
}

// --------------
// Domain Shader
// --------------

struct DS_OUT
{
    float4 vPosition : SV_POSITION0;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

void BernsteinCoeffcient5x5(float t, out float fBernstein[5])
{
    float tInv = 1.0f - t;
    fBernstein[0] = tInv * tInv * tInv * tInv;
    fBernstein[1] = 4.0f * t * tInv * tInv * tInv;
    fBernstein[2] = 6.0f * t * t * tInv * tInv;
    fBernstein[3] = 4.0f * t * t * t * tInv;
    fBernstein[4] = t * t * t * t;
}

[domain("tri")]
DS_OUT DS_Main(const OutputPatch<HS_OUT, 3> input, float3 location : SV_DomainLocation, PatchTess patch)
{
    DS_OUT output = (DS_OUT) 0.f;

    float3 localPos = input[0].vPosition * location[0] + input[1].vPosition * location[1] + input[2].vPosition * location[2];
    float3 localNormal = input[0].vNormal * location[0] + input[1].vNormal * location[1] + input[2].vNormal * location[2];
    float2 uv = input[0].vTexUV * location[0] + input[1].vTexUV * location[1] + input[2].vTexUV * location[2];

    int tileCountX = g_Tiles.x;
    int tileCountZ = g_Tiles.y;
    int mapWidth = g_HeightMapResolution.x;
    int mapHeight = g_HeightMapResolution.y;

    float2 fullUV = float2(uv.x / (float) tileCountX, uv.y / (float) tileCountZ);
    float height = g_Texture1.SampleLevel(g_Sampler_Normal, fullUV, 0).x;
    // 높이맵 높이 적용 
  //  localPos.y = height;

    float2 deltaUV = float2(1.f / mapWidth, 1.f / mapHeight);
    float2 deltaPos = float2(tileCountX * deltaUV.x, tileCountZ * deltaUV.y);

    float upHeight = g_Texture1.SampleLevel(g_Sampler_Normal, float2(fullUV.x, fullUV.y - deltaUV.y), 0).x;
    float downHeight = g_Texture1.SampleLevel(g_Sampler_Normal, float2(fullUV.x, fullUV.y + deltaUV.y), 0).x;
    float rightHeight = g_Texture1.SampleLevel(g_Sampler_Normal, float2(fullUV.x + deltaUV.x, fullUV.y), 0).x;
    float leftHeight = g_Texture1.SampleLevel(g_Sampler_Normal, float2(fullUV.x - deltaUV.x, fullUV.y), 0).x;

    float3 localTangent = float3(localPos.x + deltaPos.x, rightHeight, localPos.z) - float3(localPos.x - deltaPos.x, leftHeight, localPos.z);
    float3 localBinormal = float3(localPos.x, upHeight, localPos.z + deltaPos.y) - float3(localPos.x, downHeight, localPos.z - deltaPos.y);

    output.vPosition = Compute_FinalMatrix(localPos);
    output.vWorldPos = mul(float4(localPos, 1.f), g_WorldMatrix);
    output.vProjPos = output.vPosition;

    output.vTangent = normalize(mul(float4(localTangent, 0.f), g_WorldMatrix));
    output.vBinormal = normalize(mul(float4(localBinormal, 0.f), g_WorldMatrix));
    output.vNormal = normalize(mul(float4(localNormal, 0.f), g_WorldMatrix));

    output.vTexUV = uv;

    return output;
}
// --------------
// Pixel Shader
// --------------

struct PS_In
{
    float4 vPosition : SV_POSITION0;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vPosition : SV_Target3;
};

PS_OUT PS_Main(PS_In In) : SV_Target
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV * 20.f);
   
    VIEWPROJINFO tMainViewProj = GetViewProjInfo();
    
    Out.vNormal = normalize(In.vNormal);
    Out.vDepth = float4(In.vProjPos.w / tMainViewProj.fCamFar, In.vProjPos.z / In.vProjPos.w, 1.f, In.vPosition.w);
    Out.vPosition = In.vWorldPos;
    
    return Out;
}

#endif