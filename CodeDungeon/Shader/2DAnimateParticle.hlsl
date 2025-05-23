#ifndef _2DPARTICLE_
#define _2DPARTICLE_

#include "ShaderGrobalFunc.hlsli"


struct GROBALPARTICLEINFO
{
    int iMaxCount;
    int iAddCount;
    float fDeltaTime;
    float fAccTime;
    // ==============
  
    float fMinLifeTime;
    float fMaxLifeTime;
    float fMinSpeed;
    float fMaxSpeed;
    // ===============
   
    float fStartScaleParticle;
    float fEndScaleParticle;
    float fParticleThickness;
    int fParticleKind;
    //===============
    float3 fParticleDirection;
    float fAnimSizeX;
    
     //================
    float fAnimSizeY;
    float3 fParticlePosition;
    //=================       
    float fNextAnimTime;
    float3 fPadding;  
};

struct PARTICLE
{
    float3 vWorldPos;
    float fCurTime;
    // ===============
    float3 vWorldDir;
    float fLifeTime;
    // ==============
    int iAlive;
    float2 vAnimUV;
    float padding;
};



// All Particle

cbuffer ALLPARTICLEBUFFER : register(b13)
{
    GROBALPARTICLEINFO g_GrobalParticleInfo;
};

StructuredBuffer<PARTICLE> g_ParticleData : register(t14);

// VS_MAIN
// g_float_0    : Start Scale
// g_float_1    : End Scale
// g_tex_0      : Particle Texture

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTagent : TANGENT;
    float2 vTexUV : TEXCOORD;
    uint iInstanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 vViewPos : POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    uint iInstanceID : ID;
};


VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0.f;

    Out.vViewPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vViewPos += float4(g_ParticleData[In.iInstanceID].vWorldPos, 0.f);
    
  
    Out.vViewPos = mul(Out.vViewPos, GetViewProjInfo().mViewMatrix);
   // Out.vViewPos = mul(Out.vViewPos, GetViewProjInfo().mProjMatrix);
    Out.vTexUV = In.vTexUV;
    Out.iInstanceID = In.iInstanceID;
    return Out;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    uint iInstanceID : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_Main(point VS_OUT input[1], inout TriangleStream<GS_OUT> outputStream)
{
    GS_OUT output[4] =
    {
        (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f
    };

    VS_OUT Vertex = input[0];
    uint id = (uint) Vertex.iInstanceID;
    if (0 == g_ParticleData[id].iAlive)
        return;

    float ratio = g_ParticleData[id].fCurTime / g_ParticleData[id].fLifeTime;
    float scale = ((g_GrobalParticleInfo.fEndScaleParticle - g_GrobalParticleInfo.fStartScaleParticle) * ratio +
    g_GrobalParticleInfo.fStartScaleParticle) / 2.f;

    // View Space
    output[0].vPosition = Vertex.vViewPos + float4(-scale, scale, 0.f, 0.f);
    output[1].vPosition = Vertex.vViewPos + float4(scale, scale, 0.f, 0.f);
    output[2].vPosition = Vertex.vViewPos + float4(scale, -scale, 0.f, 0.f);
    output[3].vPosition = Vertex.vViewPos + float4(-scale, -scale, 0.f, 0.f);
    
    output[0].vWorldPos = Vertex.vWorldPos + float4(-scale, scale, 0.f, 0.f);
    output[1].vWorldPos = Vertex.vWorldPos + float4(scale, scale, 0.f, 0.f);
    output[2].vWorldPos = Vertex.vWorldPos + float4(scale, -scale, 0.f, 0.f);
    output[3].vWorldPos = Vertex.vWorldPos + float4(-scale, -scale, 0.f, 0.f);

    VIEWPROJINFO stViewProjInfo = GetViewProjInfo();
    
    // Projection Space
    output[0].vPosition = mul(output[0].vPosition, stViewProjInfo.mProjMatrix);
    output[1].vPosition = mul(output[1].vPosition, stViewProjInfo.mProjMatrix);
    output[2].vPosition = mul(output[2].vPosition, stViewProjInfo.mProjMatrix);
    output[3].vPosition = mul(output[3].vPosition, stViewProjInfo.mProjMatrix);

    
    float2 texSize = float2(g_GrobalParticleInfo.fAnimSizeX, g_GrobalParticleInfo.fAnimSizeY);
  
    float2 uvStep = float2(1.0f / texSize.x, 1.0f / texSize.y);
    float2 uvOffset = float2(g_ParticleData[id].vAnimUV.x * uvStep.x, g_ParticleData[id].vAnimUV.y * uvStep.y);

    output[0].vTexUV = float2(0.f, 0.f) * uvStep + uvOffset;
    output[1].vTexUV = float2(1.f, 0.f) * uvStep + uvOffset;
    output[2].vTexUV = float2(1.f, 1.f) * uvStep + uvOffset;
    output[3].vTexUV = float2(0.f, 1.f) * uvStep + uvOffset;
    
    output[0].iInstanceID = id;
    output[1].iInstanceID = id;
    output[2].iInstanceID = id;
    output[3].iInstanceID = id;

    // 삼각형 두 개 실행
    outputStream.Append(output[0]);
    outputStream.Append(output[1]);
    outputStream.Append(output[2]);
    outputStream.RestartStrip();

    outputStream.Append(output[0]);
    outputStream.Append(output[2]);
    outputStream.Append(output[3]);
    outputStream.RestartStrip();
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_Main(GS_OUT In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
    
 /*   if(IsColorUp(Out.vColor, 0.95f))
        discard;*/
    
    return Out;
}

#endif 