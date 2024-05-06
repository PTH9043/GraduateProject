#include "EngineDefine.h"
#include "URenderer.h"
#include "UPipeLine.h"
#include "USceneManager.h"
#include "UShader.h"
#include "UGameInstance.h"
#include "URenderTargetManager.h"
#include "UShaderConstantBuffer.h"
#include "UTransform.h"
#include "UGraphicDevice.h"
#include "UVIBufferRect.h"
#include "URenderTargetGroup.h"
#include "UTransform.h"
#include "UDefferedCamera.h"
#include "UShadowCamera.h"
#include "URenderTargetManager.h"
#include "UGlobalConstantBuffer.h"
#include "UGpuCommand.h"
#include "UPawn.h"
#include "UMethod.h"
#include "UCommand.h"


URenderer::URenderer(CSHPTRREF <UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand,
    CSHPTRREF<UGraphicDevice> _spGraphicDevice, CSHPTRREF<UPipeLine> _spPipeLine,
    CSHPTRREF<USceneManager> _spSceneManager, CSHPTRREF<URenderTargetManager> _spRenderTargetManager,
    CSHPTRREF<UComputeManager> _spComputeManager) :
    UComponent(_spDevice),
    m_spTransformConstantBuffer{ nullptr },
    m_fGrobalDeltaTime{0},
    m_spDefferedCamera{ nullptr },
    m_spShadowCamera{ nullptr },
    m_stFinalRenderTransformParam{},
    m_spGraphicDevice{ _spGraphicDevice },
    m_spPipeLine{ _spPipeLine }, m_spSceneManager{ _spSceneManager },
    m_spRenderTargetManager{ _spRenderTargetManager },
    m_spComputeManager{ _spComputeManager },
    m_spVIBufferPlane{ nullptr },
    m_sNonAlphaBlendIndex{ 0 },
    m_bTurnFog{0},
    m_spFogConstantBuffer{nullptr},
    m_spCastingCommand{ static_pointer_cast<UCommand>(_spGraphicDevice->GetGpuCommand()) }
#ifdef _USE_DEBUGGING
    , m_stRenderDebugging{}
#endif
{
}

void URenderer::Free()
{
}

HRESULT URenderer::NativeConstruct()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
  /*  {
        spGameInstance->GetGlobalConstantBuffer(GLOBAL_GLOBALDATA, m_spGlobalBuffer);
        spGameInstance->CopyToMaterialShaderParam(REF_OUT m_stGlobalParam);
    }*/
    // Rendering
    {
        m_spVIBufferPlane = static_pointer_cast<UVIBufferRect>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERRECT));
    }
    {
        // Add Shader 
        {
            m_ShaderObjects.insert(std::pair<_wstring, SHPTR<UShader>>(PROTO_RES_FINALDEFFEREDSHADER, static_pointer_cast<UShader>(
                spGameInstance->CloneResource(PROTO_RES_FINALDEFFEREDSHADER))));

            m_ShaderObjects.insert(std::pair<_wstring, SHPTR<UShader>>(PROTO_RES_BLENDDEFFEREDSHADER, static_pointer_cast<UShader>(
                spGameInstance->CloneResource(PROTO_RES_BLENDDEFFEREDSHADER))));
#ifdef _USE_DEBUGGING
            m_ShaderObjects.insert(std::pair<_wstring, SHPTR<UShader>>(PROTO_RES_DEBUG2DTARGETSHADER, static_pointer_cast<UShader>(
                spGameInstance->CloneResource(PROTO_RES_DEBUG2DTARGETSHADER))));
#endif
        }
    }
    {
        m_stFinalRenderTransformParam.mWorldMatrix._11 = spGameInstance->GetD3DViewport().Width;
        m_stFinalRenderTransformParam.mWorldMatrix._22 = spGameInstance->GetD3DViewport().Height;
        m_stFinalRenderTransformParam.mWorldMatrix = m_stFinalRenderTransformParam.mWorldMatrix.Transpose();
        {

            // Create Shadow Camera (깊이 추출용 카메라)
            UCamera::CAMDESC tDesc;
            tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::PERSPECTIVE, _float3(0.f, 0.f, 0.f),
                _float3(0.f, 0.f, 0.f),
                DirectX::XMConvertToRadians(60.0f), spGameInstance->GetD3DViewport().Width*4,
                spGameInstance->GetD3DViewport().Height*4,10.0f,2000.f,1.f);
            tDesc.stCamValue = UCamera::CAMVALUE(5.f, DirectX::XMConvertToRadians(90.f));
            tDesc.eCamType = CAMERATYPE::SHADOWLIGHT;

            VOIDDATAS vecDatas;
            vecDatas.push_back(&tDesc);

            m_spShadowCamera = static_pointer_cast<UShadowCamera>(spGameInstance->CloneActorAdd(
                PROTO_ACTOR_SHADOWCAMERA, vecDatas));
            m_spShadowCamera->SetShadowCamViewportInfo(0, 0, static_cast<long>(spGameInstance->GetD3DViewport().Width *4),
                static_cast<long>(spGameInstance->GetD3DViewport().Height * 4), 0.0f, 1.0f);
            m_spShadowCamera->SetShadowCamRectInfo(0, 0, static_cast<long>(spGameInstance->GetD3DViewport().Width * 4),
                static_cast<long>(spGameInstance->GetD3DViewport().Height * 4));
        }
        {

            // Create Deffered Camera (디퍼드용 카메라)
            UCamera::CAMDESC tDesc;
            tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::ORTHOGRAPHIC, _float3(0.f, 0.f, 0.f),
                _float3(0.f, 0.f, 0.f),
                DirectX::XMConvertToRadians(60.0f), spGameInstance->GetD3DViewport().Width,
                spGameInstance->GetD3DViewport().Height);
            tDesc.stCamValue = UCamera::CAMVALUE(5.f, DirectX::XMConvertToRadians(90.f));
            tDesc.eCamType = CAMERATYPE::DEFFERED;

            VOIDDATAS vecDatas;
            vecDatas.push_back(&tDesc);

            m_spDefferedCamera = static_pointer_cast<UDefferedCamera>(spGameInstance->CloneActorAdd(
                PROTO_ACTOR_DEFFEREDCAMERA, vecDatas));
        }
        {
            m_spFogConstantBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::FOGBOOL, static_cast<_int>(sizeof(_bool)));
        }
        m_spShadowCamera->GetTransform()->SetPos(_float3(0, 1500, 0));
        m_spShadowCamera->GetTransform()->LookAt(_float3(0, 0, 0));

        m_stFinalRenderTransformParam.iCamIndex = m_spDefferedCamera->GetCamID();
            
        // 이미 만들어진 Shader ConstnatBuffer를 가져옴
        spGameInstance->GetPreAllocatedConstantBuffer(PREALLOCATED_TRANSFORM, m_spTransformConstantBuffer);
    }
    // Initailize 
    \
        return S_OK;
}

HRESULT URenderer::NativeConstructClone(const VOIDDATAS& _vectDatas)
{
    return __super::NativeConstructClone(_vectDatas);
}

HRESULT URenderer::AddRenderGroup(const RENDERID _eID, CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn)
{
    assert(_spPawn && _spShader);
    // 쉐이더 이름 있는지 찾고
    const auto& findIter = m_arrActiveDrawRenderList[_eID].find(_spShader->GetShaderDesc().wstrShaderName);
    // 찾아서 저 쉐이더 이름이 없으면
    if (findIter == m_arrActiveDrawRenderList[_eID].end())
    {
        LIST<SHPTR<UPawn>> PawnList;
        PawnList.push_back(_spPawn);
        m_arrActiveDrawRenderList[_eID].emplace(std::pair<_wstring, LIST<SHPTR<UPawn>>>(_spShader->GetShaderDesc().wstrShaderName, PawnList));
    }
    else
    {
        findIter->second.push_back(_spPawn);
    }
    return S_OK;
}

#ifdef _USE_DEBUGGING
HRESULT URenderer::AddDebugRenderGroup(const DEBUGRENDERID _eID, CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn)
{
    RETURN_CHECK(false == m_stRenderDebugging.isShowRenderTarget, E_FAIL);
    RETURN_CHECK(nullptr == _spPawn, E_FAIL);

    const auto& findIter = m_arrDrawDebugRenderList[_eID].find(_spShader->GetShaderDesc().wstrShaderName);
    if (findIter == m_arrDrawDebugRenderList[_eID].end())
    {
        LIST<SHPTR<UPawn>> PawnList;
        PawnList.push_back(_spPawn);
        m_arrDrawDebugRenderList[_eID].emplace(std::pair<_wstring, LIST<SHPTR<UPawn>>>(_spShader->GetShaderDesc().wstrShaderName, PawnList));
        // 쉐이더 이름별로 
        // Pawn List 
        // 
    }
    else
    {
        findIter->second.push_back(_spPawn);
    }

    return S_OK;
}
#endif

void URenderer::Tick(const _double& _dTimeDelta)
{
    m_fGrobalDeltaTime += static_cast<_float>(_dTimeDelta);
}

HRESULT URenderer::Render()
{
    m_spPipeLine->UpdateViewProjMatrix(m_fGrobalDeltaTime);
    m_spPipeLine->BindViewProjMatrix(m_spCastingCommand);
    BindGrobalBuffer();
    // ============== Bind Static Buffer =============

    // Render 
  //  RenderRTs();
    RenderPriority();
    RenderShadowDepth();
    RenderNonAlphaBlend();
    RenderLights();
    RenderNonLight();
    RenderBlend();
    RenderAlphaBlend();
    RenderDistortion();
    Render2DUI();
    Render3DUI();
    RenderHDR();
    RenderEnd();

#ifdef _USE_DEBUGGING
    RenderDebug();

    m_spRenderTargetManager->RenderDebugObjects(FrameReadyDrawLast(PROTO_RES_DEBUG2DTARGETSHADER), m_spVIBufferPlane,
        m_spCastingCommand, SRV_REGISTER::T0);
#endif
    return S_OK;
}

void URenderer::ClearRenderingData()
{
    for (auto& iter : m_arrActiveDrawRenderList)
    {
        iter.clear();
    }
#ifdef  _USE_DEBUGGING
    for (auto& iter : m_arrDrawDebugRenderList)
    {
        iter.clear();
    }
#endif //  _USE_DEBUGGING
}

void URenderer::ClearAllData()
{
    m_ShaderObjects.clear();
    m_spTransformConstantBuffer.reset();
    m_spDefferedCamera.reset();
    m_spShadowCamera.reset();

    m_spGraphicDevice.reset();
    m_spPipeLine.reset();
    m_spSceneManager.reset();
    m_spRenderTargetManager.reset();
    m_spComputeManager.reset();
    m_spVIBufferPlane.reset();

    ClearRenderingData();
}

void URenderer::RenderOtherCamera()
{
    RenderNonAlphaBlend();
    RenderLights();
    RenderNonLight();
    RenderAlphaBlend();
    RenderDistortion();
    RenderHDR();
}

void URenderer::BindDefferedTransform(SHPTR<UShader> _spShader)
{
    RETURN_CHECK(nullptr == _spShader, ;);
    _spShader->BindCBVBuffer(m_spTransformConstantBuffer, &m_stFinalRenderTransformParam, GetTypeSize<TRANSFORMPARAM>());
}

void URenderer::RenderRTs()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    _uint iIndex{ 0 }, iMirror{ 0 };

    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_TARGETS])
    {
        // Render
        for (auto& Mirror : iter.second)
        {
            if (0 == iIndex)
                iMirror = (_uint)(GRAPHICRENDEROBJECT_TYPE::MIRROR_1);
            else
                iMirror = (_uint)(GRAPHICRENDEROBJECT_TYPE::MIRROR_2);

           
            m_spPipeLine->BindViewProjMatrix(m_spCastingCommand);
            BindGrobalBuffer();

            Mirror->Render(m_spCastingCommand, m_spGraphicDevice->GetTableDescriptor());
          
            iIndex = (iIndex + 1) % 2;
        }
    }

    spGameInstance->ChangeRenderCamera(MAIN_CAMERA_ID);
}

void URenderer::RenderPriority()
{
   
}

void URenderer::RenderShadowDepth()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
  
   
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::SHADOW_MAP) };
    {
        spRenderTargetGroup->WaitResourceToTarget(m_spCastingCommand);
        spRenderTargetGroup->ClearRenderTargetView(m_spCastingCommand);
        spRenderTargetGroup->OmSetRenderTargets(m_spCastingCommand);
    }
  spGameInstance->SetTemporaryViewPort(m_spShadowCamera->GetShadowCamViewPort(), m_spShadowCamera->GetShadowCamRect());

    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_SHADOW])
    {
        RenderShadowObject(iter.first, iter.second);
    }
    spRenderTargetGroup->WaitTargetToResource(m_spCastingCommand);

   spGameInstance->SetDefaultViewPort();
}

void URenderer::RenderNonAlphaBlend()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::NONALPHA_DEFFERED) };
   
    
    {
        spRenderTargetGroup->WaitResourceToTarget(m_spCastingCommand);
        spRenderTargetGroup->ClearRenderTargetView(m_spCastingCommand);
        spRenderTargetGroup->OmSetRenderTargets(m_spCastingCommand);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_PRIORITY])
    {
        RenderObject(iter.first, iter.second);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_FIRST])
    {
        RenderObject(iter.first, iter.second);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_MIDDLE])
    {
        RenderObject(iter.first, iter.second);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_LAST])
    {
        RenderObject(iter.first, iter.second);
    }
    spRenderTargetGroup->WaitTargetToResource(m_spCastingCommand);

}

void URenderer::RenderLights()
{
    // Render Light
    m_spSceneManager->RenderLight(m_spPipeLine, m_spRenderTargetManager, m_stFinalRenderTransformParam);
}

void URenderer::RenderBlend()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
  
    // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::BLEND_DEFFERED) };
    spRenderTargetGroup->WaitResourceToTarget(m_spCastingCommand);
    spRenderTargetGroup->ClearRenderTargetView(m_spCastingCommand);
    spRenderTargetGroup->OmSetRenderTargets(m_spCastingCommand);
    // Bind Shader 
    SHPTR<UShader> spShader = FindShader(PROTO_RES_BLENDDEFFEREDSHADER);
    spShader->SettingPipeLineState(m_spCastingCommand);
    spShader->SetTableDescriptor(m_spGraphicDevice->GetTableDescriptor());
    spShader->BindCBVBuffer(m_spTransformConstantBuffer, &m_stFinalRenderTransformParam, GetTypeSize<TRANSFORMPARAM>());
    {
        SHPTR<URenderTargetGroup> spNonAlpha = m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::NONALPHA_DEFFERED);
        spShader->BindSRVBuffer(SRV_REGISTER::T0, spNonAlpha->GetRenderTargetTexture(RTOBJID::NONALPHA_DIFFUSE_DEFFERED));
    }
    {
        SHPTR<URenderTargetGroup> spLightGroup = m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::LIGHTSHADE_DEFFERED);
        spShader->BindSRVBuffer(SRV_REGISTER::T1, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_SHADE_DEFFERED));
        spShader->BindSRVBuffer(SRV_REGISTER::T2, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_SPECULAR_DEFFERED));
        spShader->BindSRVBuffer(SRV_REGISTER::T3, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_AMBIENT_DEFFERED));
    }
    m_spVIBufferPlane->Render(spShader, m_spCastingCommand);
    spRenderTargetGroup->WaitTargetToResource(m_spCastingCommand);
  
}

void URenderer::RenderNonLight()
{
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NOLIGHT])
    {
        RenderObject(iter.first, iter.second);
    }
}

void URenderer::RenderAlphaBlend()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
     // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::ALPHA_DEFFERED) };
    spRenderTargetGroup->WaitResourceToTarget(m_spCastingCommand);
    spRenderTargetGroup->ClearRenderTargetView(m_spCastingCommand);
    spRenderTargetGroup->OmSetRenderTargets(m_spCastingCommand);
    {
        for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_ALPHA])
        {
            RenderObject(iter.first, iter.second);
        }
    }
    spRenderTargetGroup->WaitTargetToResource(m_spCastingCommand);
 
}

void URenderer::RenderDistortion()
{

}

void URenderer::Render3DUI()
{
}

void URenderer::Render2DUI()
{
    // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::UI2D_DEFFERED) };
    spRenderTargetGroup->WaitResourceToTarget(m_spCastingCommand);
    spRenderTargetGroup->ClearRenderTargetView(m_spCastingCommand);
    spRenderTargetGroup->OmSetRenderTargets(m_spCastingCommand);
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_2DUI])
    {
        RenderObject(iter.first, iter.second);
    }
    spRenderTargetGroup->WaitTargetToResource(m_spCastingCommand);
}

void URenderer::RenderHDR()
{

}


void URenderer::RenderEnd()
{
    // Wait AlphaBlend
    // Is Alpha
    {
        m_spGraphicDevice->OmSetDefaultRenderTarget();
        SHPTR<UShader> spDefferedShader = FindShader(PROTO_RES_FINALDEFFEREDSHADER);
        spDefferedShader->SettingPipeLineState(m_spCastingCommand);
        spDefferedShader->SetTableDescriptor(m_spGraphicDevice->GetTableDescriptor());
        spDefferedShader->BindCBVBuffer(m_spTransformConstantBuffer, &m_stFinalRenderTransformParam, GetTypeSize<TRANSFORMPARAM>());
        //  Diffuse Texture 가져와서 Bind 
        spDefferedShader->BindSRVBuffer(SRV_REGISTER::T0, m_spRenderTargetManager->
            FindRenderTargetTexture(RTGROUPID::BLEND_DEFFERED,
                RTOBJID::BLEND_SCREEN_DEFFERED));
        //  Diffuse Texture 가져와서 Bind 
        spDefferedShader->BindSRVBuffer(SRV_REGISTER::T1, m_spRenderTargetManager->
            FindRenderTargetTexture(RTGROUPID::ALPHA_DEFFERED,
                RTOBJID::ALPHA_DIFFUSE_DEFFERED));

        spDefferedShader->BindSRVBuffer(SRV_REGISTER::T2, m_spRenderTargetManager->
            FindRenderTargetTexture(RTGROUPID::NONALPHA_DEFFERED,
                RTOBJID::NONALPHA_POSITION_DEFFERED));
        {
            spDefferedShader->BindCBVBuffer(m_spFogConstantBuffer, &m_bTurnFog, sizeof(_bool));
        }
        //  Diffuse Texture 가져와서 Bind 
   /*     spDefferedShader->BindSRVBuffer(SRV_REGISTER::T2, m_spRenderTargetManager->
            FindRenderTargetTexture(RTGROUPID::UI2D_DEFFERED,
                RTOBJID::UI2D_SCREEN_DEFFERED));*/
        //  Render
        m_spVIBufferPlane->Render(spDefferedShader, m_spCastingCommand);
    }
}

#ifdef _USE_DEBUGGING
void URenderer::RenderDebug()
{
    // Render Debugging
    for (auto& Debugging : m_arrDrawDebugRenderList)
    {
        for (auto& iter : Debugging)
        {
            RenderObject(iter.first, iter.second);
        }
    }
}
#endif

SHPTR<UShader> URenderer::FindShader(const _wstring& _wstrProto)
{
    const SHADERODERS::iterator& ShaderIter = m_ShaderObjects.find(_wstrProto);
    RETURN_CHECK(m_ShaderObjects.end() == ShaderIter, nullptr);
    return ShaderIter->second;
}


void URenderer::RenderObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    // Pipelinestate setting
    spGameInstance->SettingPipeLineState(_wstrShaderName, m_spCastingCommand);
    // Shadr 필요한 Pawn끼리 list 쭉 그려
    for (auto& iter : _PawnList)
    {
        iter->Render(m_spCastingCommand, m_spGraphicDevice->GetTableDescriptor());
    }
}

void URenderer::RenderShadowObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    // Pipelinestate setting
    spGameInstance->SettingPipeLineState(_wstrShaderName, m_spCastingCommand);
    // Shadr 필요한 Pawn끼리 list 쭉 그려
    for (auto& iter : _PawnList)
    {
        iter->RenderShadow(m_spCastingCommand, m_spGraphicDevice->GetTableDescriptor());
    }
}


SHPTR<UShader> URenderer::FrameReadyDrawFinalRenderTarget(const _wstring& _wstrShaderName, const RTGROUPID _eGroupID)
{
    m_spRenderTargetManager->OmSetRenderTargets(m_spCastingCommand, _eGroupID);
    SHPTR<UShader> spShader = FindShader(_wstrShaderName);
    spShader->SettingPipeLineState(m_spCastingCommand);
    return spShader;
}

SHPTR<UShader> URenderer::FrameReadyDrawLast(const _wstring& _wstrShaderName)
{
    SHPTR<UShader> spShader = FindShader(_wstrShaderName);
    spShader->SettingPipeLineState(m_spCastingCommand);
    spShader->SetTableDescriptor(m_spGraphicDevice->GetTableDescriptor());
    return spShader;
}

void URenderer::BindGrobalBuffer()
{
   // SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    //spGameInstance->CopyToMaterialShaderParam(REF_OUT m_stGlobalParam);
    //// Setting Grobal Data
    //m_spGlobalBuffer->SettingGlobalData(m_spCastingCommand, &m_stGlobalParam, GetTypeSize<GLOBALPARAM>());
}
