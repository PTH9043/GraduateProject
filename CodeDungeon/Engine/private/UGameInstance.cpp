#include "EngineDefine.h"
#include "UGameInstance.h"

#include "URenderTargetManager.h"
#include "UResourceManager.h"
#include "UGraphicDevice.h"
#include "UShaderBufferManager.h"
#include "UPipeLine.h"
#include "UActorManager.h"


IMPLEMENT_SINGLETON(UGameInstance);

UGameInstance::UGameInstance() :
	//m_isGamming{ true },
	//m_spGraphicRenderManager{ Create<UGraphicRenderManager>() },
	m_spShaderBufferManager{ Create<UShaderBufferManager>() },
	m_spGraphicDevice(Create<UGraphicDevice>()),
	//m_spTimerManager{ Create<UTimerManager>() },
	//m_spInputManager{ Create<UInputManager>() },

	//m_spFontMananger{ Create<UFontManager>() },
	m_spActorManager(Create<UActorManager>()),
	//m_spComponentManager(Create<UComponentManager>()),
	m_spResourceManager(Create<UResourceManager>()),
	//m_spSceneManager{ Create<USceneManager>() },

	m_spRenderTargetManager(Create<URenderTargetManager>()),
	//m_spComputeManager(Create<UComputeManager>()),
	m_spPipeLine{ Create<UPipeLine>() }
	//m_spPicking{ Create<UPicking>() },
	//m_spFilePathManager{ Create<UFilePathManager>() },
	//m_spRandomManager{ Create<URandomManager>() },
	//m_spRenderer{ nullptr },
	//m_spGraphicRenderObject{ nullptr }
{

}

UGameInstance::~UGameInstance()
{
	//Free();
}

void UGameInstance::Free()
{
	//ClearOnceTypeData();
	//m_spRenderer->ClearAllData();

	//m_spRenderer.reset();
	//m_spGraphicRenderObject.reset();

	//m_isGamming = false;
	//m_spRandomManager.reset();
	//m_spFilePathManager.reset();
	//m_spPicking.reset();
	m_spPipeLine.reset();
	//m_spComputeManager.reset();
	m_spRenderTargetManager.reset();
	//m_spSceneManager.reset();
	m_spActorManager.reset();
	//m_spComponentManager.reset();
	m_spResourceManager.reset();
	//m_spFontMananger.reset();
	//m_spInputManager.reset();
	//m_spTimerManager.reset();
	m_spGraphicDevice.reset();
	m_spShaderBufferManager.reset();
	//m_spGraphicRenderManager.reset();


#if defined(_DEBUG)
	IDXGIDebug1* pdxgiDebug = NULL;
	DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), (void**)&pdxgiDebug);
	HRESULT hResult = pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	pdxgiDebug->Release();
#endif
}

HRESULT UGameInstance::ReadyInstance(const GRAPHICDESC& _stDesc, OUTPUTDATA& _stOutDesc)
{
	return S_OK;
}


/*
==================================================
GameInstance
==================================================
ResourceManager
==================================================
*/

HRESULT UGameInstance::AddPrototype(const _wstring & _wstrPrototype, const CLONETYPE & _eCloneType, CSHPTRREF<UResource> _spResource)
{
	return m_spResourceManager->AddPrototype(_wstrPrototype, _eCloneType, _spResource);
}

SHPTR<UResource> UGameInstance::CloneResource(const _wstring& _wstrPrototype, const VOIDDATAS& _stDatas)
{
	return m_spResourceManager->CloneResource(_wstrPrototype, _stDatas);
}

void UGameInstance::FindResourceProtoTag(CSHPTRREF<UResource> _spResource, _wstring& _wstrResourceTag)
{
	m_spResourceManager->FindResourceProtoTag(_spResource, _wstrResourceTag);
}

void UGameInstance::AddPipeLineState(const _wstring& _wstrName, const ComPtr<Dx12PipelineState>& _cpPipeLineState)
{
	m_spResourceManager->AddPipeLineState(_wstrName, _cpPipeLineState);
}

void UGameInstance::SettingPipeLineState(const _wstring& _wstrName, CSHPTRREF<UCommand> _spCommand)
{
	m_spResourceManager->SettingPipeLineState(_wstrName, _spCommand);
}

/*
==================================================
ResourceManager
==================================================
==================================================
GraphicDevice
==================================================
*/

const _bool UGameInstance::IsGamePaused() const
{
	return m_spGraphicDevice->IsGamePaused();
}

const _bool UGameInstance::IsScreenMinimized() const
{
	return m_spGraphicDevice->IsScreenMinimized();
}

const _bool UGameInstance::IsScreenMaximized() const
{
	return m_spGraphicDevice->IsScreenMaximized();
}

const _bool UGameInstance::IsScreenFullScreen() const
{
	return m_spGraphicDevice->IsScreenFullScreen();
}

const _bool UGameInstance::IsMssa4xState() const
{
	return m_spGraphicDevice->IsMssa4xState();
}

const _uint& UGameInstance::GetMssa4xQuality() const
{
	return m_spGraphicDevice->GetMssa4xQuality();
}

const D3D12_VIEWPORT& UGameInstance::GetD3DViewport() const
{
	return m_spGraphicDevice->GetD3DViewport();
}

const D3D12_RECT& UGameInstance::GetD3DWindowSizeRect() const
{
	return m_spGraphicDevice->GetD3DWindowSizeRect();
}

CSHPTRREF<GRAPHICDESC>UGameInstance::GetGraphicDesc() const
{
	return m_spGraphicDevice->GetGraphicDesc();
}

SHPTR<UTexture> UGameInstance::GetDefaultBackTexture() const
{
	return m_spGraphicDevice->GetDefaultBackTexture();
}

CSHPTRREF<UDevice> UGameInstance::GetDevice() const
{
	return m_spGraphicDevice->GetDevice();
}

CSHPTRREF<UGpuCommand> UGameInstance::GetGpuCommand() const
{
	return m_spGraphicDevice->GetGpuCommand();
}

CSHPTRREF<UTableDescriptor> UGameInstance::GetTableDescriptor() const
{
	return m_spGraphicDevice->GetTableDescriptor();
}

CSHPTRREF<URootSignature> UGameInstance::GetRootSignature() const
{
	return m_spGraphicDevice->GetRootSignature();
}

HRESULT UGameInstance::AddRenderTargetGroup(const RTGROUPID& _eGroupID, const std::vector<RTDESC>& _rtVec)
{
	return m_spRenderTargetManager->AddRenderTargetGroup(_eGroupID, _rtVec);
}

void UGameInstance::RemoveRenderTargetGroup(const RTGROUPID _eGroupID)
{
	m_spRenderTargetManager->RemoveRenderTargetGroup(_eGroupID);
}

// OMSetRenderTarget 
void	 UGameInstance::OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index, const _uint _iOffset)
{
	m_spRenderTargetManager->OmSetRenderTargets(_spCommand, _eGroupID, _index, _iOffset);
}
void	 UGameInstance::OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
	m_spRenderTargetManager->OmSetRenderTargets(_spCommand, _eGroupID);
}
// ClearRenderTargetView
void	 UGameInstance::ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index)
{
	m_spRenderTargetManager->ClearRenderTargetView(_spCommand, _eGroupID, _index);
}
void	 UGameInstance::ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
	m_spRenderTargetManager->ClearRenderTargetView(_spCommand, _eGroupID);
}
// Waiting
void	 UGameInstance::WaitTargetToResource(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
	m_spRenderTargetManager->WaitTargetToResource(_spCommand, _eGroupID);
}
void	 UGameInstance::WaitResourceToTarget(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
	m_spRenderTargetManager->WaitResourceToTarget(_spCommand, _eGroupID);
}


SHPTR<URenderTargetGroup> UGameInstance::FindRenderTargetGroup(const RTGROUPID& _eGroupID)
{
	return m_spRenderTargetManager->FindRenderTargetGroup(_eGroupID);
}

SHPTR<UTexture> UGameInstance::FindRenderTargetTexture(const RTGROUPID _eGroupID, const RTOBJID _eObjID)
{
	return m_spRenderTargetManager->FindRenderTargetTexture(_eGroupID, _eObjID);
}

void UGameInstance::BindGlobalBuffer(const GLOBAL_CBUFFERTYPE _eGrobalCBuffer, CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize)
{
	m_spShaderBufferManager->BindGlobalBuffer(_eGrobalCBuffer, _spCommand, _pBuffer, _iSize);
}

HRESULT UGameInstance::GetGlobalConstantBuffer(const GLOBAL_CBUFFERTYPE _eGrobalCBuffer, SHPTRREF<UGlobalConstantBuffer> _spGrobalConstantBuffer)
{
	return m_spShaderBufferManager->GetGlobalConstantBuffer(_eGrobalCBuffer, _spGrobalConstantBuffer);
}

HRESULT UGameInstance::GetPreAllocatedConstantBuffer(const PREALLOCATED_CBUFFERTYPE _ePreAllocatedCBufferType, SHPTRREF<UShaderConstantBuffer> _spShaderConstantBuffer)
{
	return m_spShaderBufferManager->GetPreAllocatedConstantBuffer(_ePreAllocatedCBufferType, _spShaderConstantBuffer);
}

void UGameInstance::RegisterCameraInPipeline(CSHPTRREF<UCamera> _spCamera, CAMID& _iCamID, const VIEWPROJMATRIX& _stViewProjMatrix, const CAMERATYPE _eType)
{
	m_spPipeLine->RegisterCameraInPipeline(_spCamera, _iCamID, _stViewProjMatrix, _eType);
}

void UGameInstance::RemoveCameraInPipeLine(CAMID _CamID)
{
	m_spPipeLine->RemoveCameraInPipeLine(_CamID);
}
_bool UGameInstance::IsFrustomContains(const _float3& _vPos, const _float _fRadius, const CAMID& _iCamID)
{
	return m_spPipeLine->IsFrustomContains(_vPos, _fRadius, _iCamID);
}

const _float4x4 UGameInstance::GetMainCamViewMatrix() const
{
	return m_spPipeLine->GetMainCamViewMatrix();
}

const _float4x4& UGameInstance::GetMainCamProjMatrix() const
{
	return m_spPipeLine->GetMainCamProjMatrix();
}

const _float3& UGameInstance::GetMainCamPosition()
{
	return m_spPipeLine->GetMainCamPosition();
}
const _float4x4 UGameInstance::GetCamViewMatrix(const CAMID& _iID) const
{
	return m_spPipeLine->GetCamViewMatrix(_iID);
}

const _float4x4& UGameInstance::GetCamProjMatrix(const CAMID& _iID) const
{
	return m_spPipeLine->GetCamProjMatrix(_iID);
}

const _float3& UGameInstance::GetCameraPosition(const CAMID& _iID)
{
	return m_spPipeLine->GetCameraPosition(_iID);
}

const _float UGameInstance::GetCamFar(const CAMID& _iID)
{
	return m_spPipeLine->GetCamFar(_iID);
}

void UGameInstance::ChangeRenderCamera(CSHPTRREF<UCamera> _spCamera)
{
	m_spPipeLine->ChangeRenderCamera(_spCamera);
}

void UGameInstance::ChangeRenderCamera(CAMID _iID)
{
	m_spPipeLine->ChangeRenderCamera(_iID);
}

void UGameInstance::AddRenderCamList(CSHPTRREF<UCamera> _spCamera)
{
	m_spPipeLine->AddRenderCamList(_spCamera);
}

void UGameInstance::AddRenderCamList(CAMID _iID)
{
	m_spPipeLine->AddRenderCamList(_iID);
}

const CAMID UGameInstance::GetRenderCamID() const
{
	return m_spPipeLine->GetRenderCamID();
}

/*
==================================================
ActorManager
==================================================
*/



const CLONEARR& UGameInstance::GetClonesArr() const
{
	return m_spActorManager->GetClonesArr();
}

HRESULT UGameInstance::AddPrototype(const _wstring& _wstrName, CSHPTRREF<UActor> _spActor)
{
	return m_spActorManager->AddPrototypes(_wstrName, _spActor);
}

void UGameInstance::CloneActor(const _wstring& _wstrProto, const VOIDDATAS& _stDatas)
{
	m_spActorManager->CloneActor(_wstrProto, _stDatas);
}

SHPTR<UActor> UGameInstance::CloneActorAdd(const _wstring& _wstrProto, const VOIDDATAS& _stDatas)
{
	return m_spActorManager->CloneActorAdd(_wstrProto, _stDatas);
}

SHPTR<UActor> UGameInstance::CloneActorAddAndNotInLayer(const _wstring& _wstrProto, const VOIDDATAS& _stDatas)
{
	return m_spActorManager->CloneActorAddAndNotInLayer(_wstrProto, _stDatas);
}

void UGameInstance::RemoveActor(CSHPTRREF<UActor> _spActor)
{
	m_spActorManager->RemoveActor(_spActor);
}

