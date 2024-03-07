#include "UGameInstance.h"
//#include "UGraphicRenderManager.h"
#include "UShaderBufferManager.h"
#include "UGraphicDevice.h"
#include "UTimerManager.h"
#include "UInputManager.h"
#include "UThreadManager.h"
//#include  "UFontManager.h"
#include "UActorManager.h"
#include "UComponentManager.h"
#include "UResourceManager.h"
#include "USceneManager.h"

#include "UPipeLine.h"
#include "URenderTargetManager.h"
#include "UFilePathManager.h"
#include "UNetworkManager.h"
#include "UAudioSystemManager.h"
#include "UCharacterManager.h"

#include "URenderer.h"


#include "URootSignature.h"
#include "UComputeRootSignature.h"
#include "UShaderConstantBuffer.h"
#include "UGlobalConstantBuffer.h"

#include "UDevice.h"
#include "UGpuCommand.h"
#include "UTableDescriptor.h"
#include "UComputeCommand.h"

#include "UDefferedCamera.h"
#include "UDefaultCube.h"
#include "UDefaultDebugging.h"
#include "UDefaultCell.h"

#include "UShader.h"
#include "UComputeShader.h"

#include "UVIBufferRect.h"
#include "UVIBufferPoint.h"
//#include "UVIBufferTriangle.h"
#include "UVIBufferSphere.h"
#include "UVIBufferCube.h"
#include "UVIBufferPlane.h"
#include "UVIBufferGrid.h"
//#include "UVIBufferSkyBox.h"

#include "UParticleSystem.h"

#include "UStageManager.h"
#include "UStage.h"

//#include "URectTransform.h"
//#include "USkyBox.h"
//#include "UTerrain.h"
#include "UParticle.h"
#include "UCollider.h"
#include "UAnimatedParticle.h"
//#include "UMirror.h"
//#include "UScreenRenderObj.h"
//#include "UMirrorCamera.h"

#include "UNavigation.h"
#include "URegion.h"

IMPLEMENT_SINGLETON(UGameInstance);

UGameInstance::UGameInstance() :
	m_isGamming{ true },
	m_spGraphicDevice(Create<UGraphicDevice>()),
	m_spShaderBufferManager{ Create<UShaderBufferManager>() },
	m_spTimerManager{ Create<UTimerManager>() },
	m_spInputManager{ Create<UInputManager>() },
	m_spThreadManager{Create<UThreadManager>()},

	//m_spFontMananger{ Create<UFontManager>() },
	m_spActorManager(Create<UActorManager>()),
	m_spComponentManager(Create<UComponentManager>()),
	m_spResourceManager(Create<UResourceManager>()),
	m_spSceneManager{ Create<USceneManager>() },

	m_spRenderTargetManager(Create<URenderTargetManager>()),
	//m_spComputeManager(Create<UComputeManager>()),
	m_spPipeLine{ Create<UPipeLine>() },
	//m_spPicking{ Create<UPicking>() },
	m_spFilePathManager{ Create<UFilePathManager>() },
	//m_spRandomManager{ Create<URandomManager>() },
	m_spAudioSystemManager{ Create<UAudioSystemManager>() },
	m_spNetworkManager{Create<UNetworkManager>()},
	m_spCharacterManager{Create<UCharacterManager>()},
	m_spRenderer{ nullptr },
	m_spStageManager{Create<UStageManager>()}
	//m_spGraphicRenderObject{ nullptr }
{
}

UGameInstance::~UGameInstance()
{
}

void UGameInstance::Free()
{
	//ClearOnceTypeData();
	m_spRenderer->ClearAllData();
	m_spRenderer.reset();
	//m_spGraphicRenderObject.reset();

	//m_isGamming = false;
	//m_spRandomManager.reset();
	m_spCharacterManager.reset();
	m_spNetworkManager.reset();
	m_spAudioSystemManager.reset();
	m_spFilePathManager.reset();
	//m_spPicking.reset();
	m_spPipeLine.reset();
	//m_spComputeManager.reset();
	m_spRenderTargetManager.reset();
	m_spSceneManager.reset();
	m_spResourceManager.reset();
	m_spComponentManager.reset();
	m_spActorManager.reset();
	m_spThreadManager.reset();
	m_spInputManager.reset();
	m_spTimerManager.reset();
	m_spGraphicDevice.reset();
	m_spShaderBufferManager.reset();
	m_spStageManager.reset();
}

HRESULT UGameInstance::ReadyInstance(const GRAPHICDESC& _stDesc, OUTPUTDATA& _stOutDesc)
{
	RETURN_CHECK_FAILED(m_spGraphicDevice->ReadyGraphicDevice(_stDesc, _stOutDesc), E_FAIL);
	RETURN_CHECK_FAILED(m_spShaderBufferManager->ReadyShaderBufferManager(_stOutDesc.wpDevice.lock()), E_FAIL);
	RETURN_CHECK_FAILED(m_spInputManager->ReadyInpuDevice(m_spGraphicDevice->GetGraphicDesc()), E_FAIL);
	RETURN_CHECK_FAILED(m_spRenderTargetManager->ReadyRenderTarget(m_spGraphicDevice, m_spGraphicDevice->GetDevice()), E_FAIL);

	RETURN_CHECK_FAILED(ReadyRenderTarget(_stOutDesc), E_FAIL);
	RETURN_CHECK_FAILED(ReadyResource(_stOutDesc), E_FAIL);
	RETURN_CHECK_FAILED(ReadyComp(_stOutDesc), E_FAIL);
	RETURN_CHECK_FAILED(ReadyActor(_stOutDesc), E_FAIL);

	RETURN_CHECK_FAILED(m_spActorManager->ReadyActorManager(m_spRenderer), E_FAIL);
	RETURN_CHECK_FAILED(m_spSceneManager->ReadySceneManager(this), E_FAIL);
	
	RETURN_CHECK_FAILED(m_spPipeLine->ReadyPipeLine(this), E_FAIL);
	RETURN_CHECK_FAILED(m_spAudioSystemManager->ReadyAudioSystemManager(this), E_FAIL);

	m_isGamming = true;
	return S_OK;
}

HRESULT UGameInstance::CreateGraphicsShader(const _wstring& _wstrProtoName, const CLONETYPE _eCloneType,
	const SHADERDESC& _stShaderDesc, const GRAPHICRENDEROBJECT_TYPE _eType)
{
	
	SHPTR<UShader> pShader = CreateConstructorToNative<UShader>(
		m_spGraphicDevice->GetDevice(), m_spGraphicDevice->GetRootSignature(),
		_stShaderDesc
	);
	RETURN_CHECK(nullptr == pShader, E_FAIL);
	// AddPrototype
	return AddPrototype(_wstrProtoName, _eCloneType, pShader);
}

HRESULT UGameInstance::CreateComputeShader(const _wstring& _wstrProtoName, const CLONETYPE _eCloneType, const SHADERDESC& _stShaderDesc)
{
	RETURN_CHECK(_stShaderDesc.pElement != nullptr, E_FAIL);

	// Shader 
	SHPTR<UComputeShader> pShader = CreateConstructorToNative<UComputeShader>(
		m_spGraphicDevice->GetDevice(), m_spGraphicDevice->GetComputeRootSignature(),
		_stShaderDesc
		);
	RETURN_CHECK(nullptr == pShader, E_FAIL);
	// AddPrototype
	return AddPrototype(_wstrProtoName, _eCloneType, pShader);
}

void UGameInstance::OtherFrame(const _double& _dTimeDelta, const WPARAM& _wParam)
{
	m_spInputManager->InvokeKeyMethods((_ubyte)_wParam, _dTimeDelta);
}

void UGameInstance::AwakeTick()
{
	m_spInputManager->KeyTick();
	m_spInputManager->MouseTick();
	// m_spPicking->TickRayInWorldSpace(this);
	m_spPipeLine->FrustomTick();
	m_spRenderer->ClearRenderingData();
}

void UGameInstance::Tick(const _double& _dTimeDelta)
{
	m_spRenderer->Tick(_dTimeDelta);
	m_spSceneManager->Tick(_dTimeDelta);
	m_spActorManager->Tick(_dTimeDelta);
}

void UGameInstance::LateTick(const _double& _dTimeDelta)
{
	m_spSceneManager->LateTick(_dTimeDelta);
	m_spActorManager->LateTick(_dTimeDelta);

	m_spSceneManager->CollisionTick(_dTimeDelta);
}

void UGameInstance::RenderBegin()
{
	m_spGraphicDevice->MainRenderBegin();
	m_spRenderer->Render();
//	m_spFontMananger->Render();
}

void UGameInstance::RenderEnd()
{
	/* Gpu 동기화 시키는 부분 */
	m_spGraphicDevice->MainRenderEnd();
}

void UGameInstance::SetImGuiContext(ImGuiContext* _pContext)
{
	ImGui::SetCurrentContext(_pContext);
}

HRESULT UGameInstance::OnWindowResize(const _uint& _iWinSizeX, const _uint& _iWinSizeY, const GRAPHICDESC::WINMODE _eWindowMode)
{
	RETURN_CHECK_FAILED(m_spGraphicDevice->OnResize(_iWinSizeX, _iWinSizeY, _eWindowMode), E_FAIL);
	m_spRenderTargetManager->OnResizeWindow(m_spGraphicDevice);
	//m_spPicking->ReadyPickingDesc(m_spGraphicDevice->GetGraphicDesc());
#ifdef _USE_DEBUGGING
	//m_spComputeManager->OnResizeDebugRenderObject(m_spGraphicDevice);
#endif 
	return S_OK;
}

void UGameInstance::ClearOnceTypeData()
{
	m_spRenderer->ClearRenderingData();
	m_spActorManager->ClearOnceTypeData();
	m_spResourceManager->ClearOnceTypeData();
	m_spPipeLine->ClearOneTypeCamera();
}


/*
-----------------------------
UGameInstance
-----------------------------
UGraphicDevice
-----------------------------
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

/*
==================================================
GraphicDevice
==================================================
ShaderBufferManager
==================================================
*/

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


/*
==================================================
ShaderBufferManager
==================================================
UTimerManager
==================================================
*/

HRESULT UGameInstance::CreateTimer(const _wstring& _wstrName)
{
	return m_spTimerManager->CreateTimer(_wstrName);
}

SHPTR<UTimer> UGameInstance::CreateTimerAdd(const _wstring& _wstrName)
{
	return m_spTimerManager->CreateTimerAdd(_wstrName);
}

HRESULT UGameInstance::RemoveTimer(const _wstring& _wstrName)
{
	return m_spTimerManager->RemoveTimer(_wstrName);
}

/*
==================================================
UTimerManager
==================================================
UInputManager
==================================================
*/

void UGameInstance::AddKeyMethod(_ubyte _bFirstKeyID, const _wstring& _wstrSecondsKeyName, const KEYACTIONFUNC& _keyMethod)
{
	m_spInputManager->AddKeyMethod(_bFirstKeyID, _wstrSecondsKeyName, _keyMethod);
}

void UGameInstance::DeleteKeyMethod( _ubyte _bFirstKeyID, const _wstring& _wstrSecondsKeyName)
{
	m_spInputManager->DeleteKeyMethod(_bFirstKeyID, _wstrSecondsKeyName);
}

_bool UGameInstance::GetDIKeyDown( _ubyte _bKeyID)
{
	return m_spInputManager->GetDIKeyDown(_bKeyID);
}

_bool UGameInstance::GetDIKeyUp( _ubyte _bKeyID)
{
	return m_spInputManager->GetDIKeyUp(_bKeyID);
}

_bool UGameInstance::GetDIKeyPressing( _ubyte _bKeyID)
{
	return m_spInputManager->GetDIKeyPressing(_bKeyID);
}

_bool UGameInstance::GetDIMBtnDown(_ubyte _eMouseBtn)
{
	return m_spInputManager->GetDIMBtnDown(_eMouseBtn);
}

_bool UGameInstance::GetDIMBtnUp(_ubyte _eMouseBtn)
{
	return m_spInputManager->GetDIMBtnUp(_eMouseBtn);
}

_bool UGameInstance::GetDIMBtnPressing(_ubyte _eMouseBtn)
{
	return m_spInputManager->GetDIMBtnPressing(_eMouseBtn);
}

_long UGameInstance::GetDIMMoveState(_ubyte _eMouseMove)
{
	return m_spInputManager->GetDIMMoveState(_eMouseMove);
}

_float2 UGameInstance::GetMousePosition()
{
	return m_spInputManager->GetMousePosition();
}

/*
==================================================
UInputManager
==================================================
UThreadManager
==================================================
*/

void UGameInstance::RegisterFuncToRegister(const THREADFUNC& _CallBack, void* _pData)
{
	m_spThreadManager->RegisterFuncToRegister(_CallBack, _pData);
}

void UGameInstance::JoinRegister()
{
	m_spThreadManager->JoinRegister();
}

void UGameInstance::DetachRegister()
{
	m_spThreadManager->DetachRegister();
}


/*
==================================================
ThreadManager
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

/*
==================================================
ActorManager
==================================================
ComponentManager
==================================================
*/

HRESULT UGameInstance::AddPrototype(const _wstring& _wstrPrototypes, CSHPTRREF<UComponent> _spComponent)
{
	return m_spComponentManager->AddPrototype(_wstrPrototypes, _spComponent);
}

SHPTR<UComponent> UGameInstance::CloneComp(const _wstring& _wstrPrototypes, const VOIDDATAS& _stDatas)
{
	return m_spComponentManager->CloneComp(_wstrPrototypes, _stDatas);
}

/*
==================================================
ComponentManager
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
SceneManager
==================================================
*/

void UGameInstance::OutLightControlInfo(LIGHTPARAM& _stLightControl)
{
	m_spSceneManager->OutLightControlInfo(_stLightControl);
}

void UGameInstance::ChangeLightCamIndex(const CAMID& _iID)
{
	m_spSceneManager->ChangeLightCamIndex(_iID);
}

CSHPTRREF<UScene> UGameInstance::GetCurScene() const
{
	return m_spSceneManager->GetScene();
}

void UGameInstance::RegisterScene(CSHPTRREF<UScene> _spScene)
{
	m_spSceneManager->RegisterScene(_spScene);
}

HRESULT UGameInstance::AddLight(const LIGHTINFO& _stInfo)
{
	return m_spSceneManager->AddLight(_stInfo);
}

void UGameInstance::OutLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex, SHPTR<ULight>& _spLight)
{
	m_spSceneManager->OutLight(_eLightType, _iIndex, _spLight);
}

HRESULT UGameInstance::ActiveLIght(const LIGHTTYPE& _eLightType, const _uint& _iIndex, const _bool& _isActive)
{
	return m_spSceneManager->ActiveLIght(_eLightType, _iIndex, _isActive);
}

HRESULT UGameInstance::DeleteLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex)
{
	return m_spSceneManager->DeleteLight(_eLightType, _iIndex);
}

HRESULT UGameInstance::ClearLight()
{
	return m_spSceneManager->ClearLight();
}


/*
==================================================
SceneManager
==================================================
RenderTargetManager
==================================================
*/

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

/*
==================================================
RenderTargetManager
==================================================
PipeLine
==================================================
*/

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
PipeLine
==================================================
FilePathManager
==================================================
*/

SHPTR<FILEGROUP> UGameInstance::FindFolder(const PATHS& _vecFolderList)
{
	return m_spFilePathManager->FindFolder(_vecFolderList);
}

SHPTR<FILEGROUP> UGameInstance::FindFolder(const _wstring& _wstrFindName, const _wstring& _wstrParentsFolderName)
{
	return m_spFilePathManager->FindFolder(_wstrFindName, _wstrParentsFolderName);
}

VECTOR<SHPTR<FILEGROUP>> UGameInstance::FindSameAllFolder(const _wstring& _wstrFindFolderName)
{
	return m_spFilePathManager->FindSameAllFolder(_wstrFindFolderName);
}

HRESULT UGameInstance::LoadFirstFolder(const _wstring& _wstrFilePath)
{
	return m_spFilePathManager->LoadFirstFolder(_wstrFilePath);
}
/*
==================================================
FilePathManager
==================================================
AudioSystemManager
==================================================
*/

const AUDIOSYSTEMCONTAINER& UGameInstance::GetAudioSystemContainer() const
{
	return m_spAudioSystemManager->GetAudioSystemContainer();
}

SHPTR<UAudioSystem> UGameInstance::GetAudioSystem(const SOUNDTYPE _SoundType)
{
	return m_spAudioSystemManager->GetAudioSystem(_SoundType);
}

HRESULT UGameInstance::CreateAudioSystemAndRegister(SOUNDTYPE _SoundType, const _wstring& _wstrSoundFolderPath)
{
	return m_spAudioSystemManager->CreateAudioSystemAndRegister(this, _SoundType, _wstrSoundFolderPath);
}

HRESULT UGameInstance::CreateAudioSystemAndRegister(SOUNDTYPE _SoundType, CSHPTRREF<FILEGROUP> _spSoundFileGroup)
{
	return m_spAudioSystemManager->CreateAudioSystemAndRegister(this, _SoundType, _spSoundFileGroup);
}

HRESULT UGameInstance::CreateAudioSystemToFolderNameAndRegister(SOUNDTYPE _SoundType, const _wstring& _wstrSoundFolderName)
{
	return m_spAudioSystemManager->CreateAudioSystemToFolderNameAndRegister(this, _SoundType, _wstrSoundFolderName);
}

void UGameInstance::SoundPlay(const _wstring& _wstrSoundName)
{
	m_spAudioSystemManager->Play(_wstrSoundName);
}

void UGameInstance::SoundPlay(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate)
{
	m_spAudioSystemManager->Play(_wstrSoundName, _fVolumeUpdate);
}

void UGameInstance::SoundPlayBGM(const _wstring& _wstrSoundName)
{
	m_spAudioSystemManager->PlayBGM(_wstrSoundName);
}

void UGameInstance::SoundPlayBGM(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate)
{
	m_spAudioSystemManager->PlayBGM(_wstrSoundName, _fVolumeUpdate);
}

void UGameInstance::StopSound(const _wstring& _wstrSoundName)
{
	m_spAudioSystemManager->Stop(_wstrSoundName);
}

void UGameInstance::UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTargetTransform)
{
	m_spAudioSystemManager->UpdateSound3D(_wstrSoundName, _vSoudPos, _vSoundVelocity, _spTargetTransform);
}

void UGameInstance::VolumeUpdate(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate)
{
	m_spAudioSystemManager->VolumeUpdate(_wstrSoundName, _fVolumeUpdate);
}

void UGameInstance::ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance)
{
	m_spAudioSystemManager->ChangeMinMaxDistance3D(_wstrSoundName, _fMinDistance, _fMaxDistance);
}

SHPTR<USound> UGameInstance::BringSound(const _int _Index)
{
	return m_spAudioSystemManager->BringSound(_Index);
}

SHPTR<USound> UGameInstance::BringSound(const _wstring& _wstrSoundName)
{
	return m_spAudioSystemManager->BringSound(_wstrSoundName);
}

/*
==================================================
AudioSystemManager
==================================================
NetworkManager
==================================================
*/


HRESULT UGameInstance::StartNetwork(CSHPTRREF<UNetworkBaseController> _spNetworkBaseController)
{
	return m_spNetworkManager->StartNetwork(_spNetworkBaseController);
}

void UGameInstance::InsertProcessedDataToContainer(void* _pData, size_t _Size, _int _DataType)
{
	m_spNetworkManager->InsertProcessedDataToContainer(_pData, _Size, _DataType);
}

void UGameInstance::PopProcessedData(POINTER_IN UProcessedData* _pData)
{
	m_spNetworkManager->PopProcessedData(_pData);
}

void UGameInstance::SendTcpPacket(_char* _pPacket, _short _PacketType, _short _PacketSize)
{
	m_spNetworkManager->SendTcpPacket(_pPacket, _PacketType, _PacketSize);
}

void UGameInstance::NetworkEnd()
{
	m_spNetworkManager.reset();
}

/*
==================================================
NetworkManager
==================================================
CharacterManager
==================================================
*/

CSHPTRREF<UCharacter> UGameInstance::GetCurrPlayer() const
{
	return m_spCharacterManager->GetCurrPlayer();
}

void UGameInstance::ReigsterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer)
{
	m_spCharacterManager->ReigsterCurrentPlayer(_spCurrentPlayer);
}

/*
==================================================
CharacterManager
==================================================
StageManager
==================================================
*/

SHPTR<UStage> UGameInstance::GetStage()
{
	return m_spStageManager->GetStage();
}

/*
==================================================
StageManager
==================================================
ReadyDatas
==================================================
*/

HRESULT UGameInstance::ReadyResource(const OUTPUTDATA & _stData)
{
	// VIBuffer
	{
		AddPrototype(PROTO_RES_VIBUFFERPOINT, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferPoint>(
			_stData.wpDevice.lock()));

		AddPrototype(PROTO_RES_VIBUFFERRECT, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferRect>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::GENERIC));

		AddPrototype(PROTO_RES_VIBUFFERNORMALRECT, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferRect>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::NORMAL));
		
		AddPrototype(PROTO_RES_VIBUFFERPLANE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferPlane>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::GENERIC));

		AddPrototype(PROTO_RES_VIBUFFERNORMALPLANE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferPlane>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::NORMAL));

		AddPrototype(PROTO_RES_VIBUFFERGRID, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferGrid>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::GENERIC));

		AddPrototype(PROTO_RES_VIBUFFERNORMALGRID, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferGrid>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::NORMAL));

		AddPrototype(PROTO_RES_VIBUFFERSHPHERE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferSphere>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::GENERIC));

		AddPrototype(PROTO_RES_VIBUFFERNORMALSPHERE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferSphere>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::NORMAL));

		AddPrototype(PROTO_RES_VIBUFFERCUBE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferCube>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::GENERIC));

		AddPrototype(PROTO_RES_VIBUFFERNORMALCUBE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferCube>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::NORMAL));

	/*	

		AddPrototype(PROTO_RES_VIBUFFERTRIANGLE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferTriangle>(
			_stData.wpDevice.lock()));

		AddPrototype(PROTO_RES_VIBUFFERNORMALPLANE, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferPlane>(
			_stData.wpDevice.lock(), VIBUFFERTYPE::NORMAL));

		AddPrototype(PROTO_RES_VIBUFFERSKYBOX, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UVIBufferSkyBox>(
			_stData.wpDevice.lock()));*/
	}
	// Graphics Shader
	{
		// Finals 
		{
			CreateGraphicsShader(PROTO_RES_FINALDEFFEREDSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"FinalDeffered", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN }, RASTERIZER_TYPE::CULL_BACK, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE));

			CreateGraphicsShader(PROTO_RES_BLENDDEFFEREDSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"BlendDeffered", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN }, RENDERFORMATS{
					DXGI_FORMAT_R32G32B32A32_FLOAT }, RASTERIZER_TYPE::CULL_BACK,
					DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE));

			CreateGraphicsShader(PROTO_RES_DEBUG2DTARGETSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"Debug2DTarget", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN }, RASTERIZER_TYPE::CULL_BACK, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE));

			CreateGraphicsShader(PROTO_RES_SCREENRENDERONBJSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"ScreenRenderObj", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN }, RENDERFORMATS{
					DXGI_FORMAT_R32G32B32A32_FLOAT }, RASTERIZER_TYPE::CULL_BACK,
					DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE));
		}
		//// Light Shadow
		//{
		//	CreateGraphicsShader(PROTO_RES_LIGHTDIRECTIONSHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"LightDirection", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN },
		//			RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM,
		//			DXGI_FORMAT_R8G8B8A8_UNORM },
		//			RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST
		//		), GRAPHICRENDEROBJECT_TYPE::LIGHT);

		//	CreateGraphicsShader(PROTO_RES_LIGHTPOINTSHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"LightPoint", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN },
		//			RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM,
		//			DXGI_FORMAT_R8G8B8A8_UNORM },
		//			RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST,
		//			BLEND_TYPE::ADD_BLEND), GRAPHICRENDEROBJECT_TYPE::LIGHT);
		//}
		// NonAlpha
		{
			CreateGraphicsShader(PROTO_RES_MODELSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"Model", VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN },
					RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
					DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT
					}));

		//	CreateGraphicsShader(PROTO_RES_EQUIPMENTSHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"EquipmentModel", VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN },
		//			RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
		//			DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT
		//			}));

			CreateGraphicsShader(PROTO_RES_ANIMMODELSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"AnimModel", VTXANIMMODEL_DECLARATION::Element, VTXANIMMODEL_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN },
					RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
					DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT
					}));

		//	CreateGraphicsShader(PROTO_RES_3DNORMALCUBESHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"3DNormalCube", VTXNORMALCUBE_DECLARATION::Element, VTXNORMALCUBE_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN },
		//			RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
		//			DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT
		//			}));

			CreateGraphicsShader(PROTO_RES_NORMALOBJECTSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"NormalObject", VTXNORMAL_DECLARATION::Element, VTXNORMAL_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN },
					RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
					DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT
					}));

		//	CreateGraphicsShader(PROTO_RES_MIRRORSHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"Mirror", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN },
		//			RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
		//			DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT
		//			}));

		//	CreateGraphicsShader(PROTO_RES_WATERSHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"Water", VTXPOINT_DELCARTION::Element, VTXPOINT_DELCARTION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN },
		//			RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
		//			DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT },
		//			RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_NO_WRITE, BLEND_TYPE::ALPHA_BLEND));
		}
		//// Alpha
		{
			CreateGraphicsShader(PROTO_RES_PARTICLE2DSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"2DParticle", VTXPOINT_DELCARTION::Element, VTXPOINT_DELCARTION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN, GS_MAIN },
					RENDERFORMATS{ DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8A8_UNORM,	DXGI_FORMAT_R16G16B16A16_FLOAT },
					RASTERIZER_TYPE::CULL_BACK, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE, BLEND_TYPE::ALPHA_BLEND,
					D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST));

			CreateGraphicsShader(PROTO_RES_2DANIMATEPARTICLESHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"2DAnimateParticle", VTXPOINT_DELCARTION::Element, VTXPOINT_DELCARTION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN, GS_MAIN },
					RENDERFORMATS{ DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8A8_UNORM,	DXGI_FORMAT_R16G16B16A16_FLOAT },
					RASTERIZER_TYPE::CULL_BACK, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE, BLEND_TYPE::ALPHA_BLEND,
					D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST));
		}
		//// SkyBox
		//{
		//	CreateGraphicsShader(PROTO_RES_SKYBOXSHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"SkyBox", VTXSKYBOX_DECLARATION::Element, VTXSKYBOX_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN }, RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
		//			DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT },
		//			RASTERIZER_TYPE::CULL_FRONT, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST));
		//}
		// Ect 
		{
			CreateGraphicsShader(PROTO_RES_DEBUGGINGDEFAULTSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"DebugDefaultObject", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN },
					RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
					DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT },
					RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS, BLEND_TYPE::ALPHA_BLEND));
		}
		{
			CreateGraphicsShader(PROTO_RES_RECTSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"Rect", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN }));
		}
		{
			CreateGraphicsShader(PROTO_RES_GRIDSHADER, CLONETYPE::CLONE_STATIC,
				SHADERDESC(L"Grid", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
					SHADERLIST{ VS_MAIN, PS_MAIN }, RASTERIZER_TYPE::WIREFRAME));
		}

		//// Create  Tess
		//{
		//	CreateGraphicsShader(PROTO_RES_TERRAINTESSALATIONSHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"TerrainTess", VTXNORMAL_DECLARATION::Element, VTXNORMAL_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN, DS_MAIN, HS_MAIN },
		//			RENDERFORMATS{ DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32G32B32A32_FLOAT,
		//			DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT },
		//			RASTERIZER_TYPE::WIREFRAME, DEPTH_STENCIL_TYPE::LESS, BLEND_TYPE::DEFAULT,
		//			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST));
		//}
		//// Ect 
		//{
		//	CreateGraphicsShader(PROTO_RES_2DUISHADER, CLONETYPE::CLONE_STATIC,
		//		SHADERDESC(L"UI2D", VTXDEFAULT_DECLARATION::Element, VTXDEFAULT_DECLARATION::iNumElement,
		//			SHADERLIST{ VS_MAIN, PS_MAIN }, RENDERFORMATS{
		//			DXGI_FORMAT_R32G32B32A32_FLOAT }, RASTERIZER_TYPE::CULL_BACK,
		//			DEPTH_STENCIL_TYPE::LESS));
		//}
		}
	//// Compute Shader 
	{
		CreateComputeShader(PROTO_RES_COMPUTEANIMATIONSHADER, CLONETYPE::CLONE_STATIC,
			SHADERDESC{ L"ComputeAnimation" });

		CreateComputeShader(PROTO_RES_COMPUTEPARTICLE2DSHADER, CLONETYPE::CLONE_STATIC,
			SHADERDESC{ L"Compute2DParticle" });
	}

	// Particle System 
	{
		AddPrototype(PROTO_RES_PARTICLESYSTEM, CLONETYPE::CLONE_STATIC, CreateConstructorToNative<UParticleSystem>(_stData.wpDevice.lock()));
	}

	return S_OK;
}

HRESULT UGameInstance::ReadyComp(const OUTPUTDATA& _stData)
{
	// Add Deffered Camera
	{
		AddPrototype(PROTO_ACTOR_DEFFEREDCAMERA, CreateConstructorToNative<UDefferedCamera>(
			_stData.wpDevice.lock(), LAYER_CAM, CLONETYPE::CLONE_STATIC));
	}
	{
		m_spRenderer = CreateConstructorToNative<URenderer>(_stData.wpDevice.lock(), _stData.wpGpuCmd.lock(), m_spGraphicDevice,
			m_spPipeLine, m_spSceneManager, m_spRenderTargetManager, nullptr);
		AddPrototype(PROTO_COMP_RENDERER, m_spRenderer);
	}
	//{
	//	// Add Rect Transform
	//	AddPrototype(PROTO_COMP_RECTTRANSFORM, CreateConstructorToNativeNotMsg<URectTransform>(_stData.wpDevice.lock()));
	//}
	{
		AddPrototype(PROTO_COMP_SPHERECOLLIDER, CreateConstructorToNativeNotMsg<UCollider>(_stData.wpDevice.lock(), UCollider::TYPE_SPHERE));
		AddPrototype(PROTO_COMP_ABBCOLLIDER, CreateConstructorToNativeNotMsg<UCollider>(_stData.wpDevice.lock(), UCollider::TYPE_AABB));
		AddPrototype(PROTO_COMP_OBBCOLLIDER, CreateConstructorToNativeNotMsg<UCollider>(_stData.wpDevice.lock(), UCollider::TYPE_OBB));
	}
	//Add Navigation
	{
		AddPrototype(PROTO_COMP_NAVIGATION, CreateConstructorToNativeNotMsg<UNavigation>(_stData.wpDevice.lock()));
	}
	//Add Region
	{
		AddPrototype(PROTO_COMP_REGION, CreateConstructorToNativeNotMsg<URegion>(_stData.wpDevice.lock()));
	}
	return S_OK;
}

HRESULT UGameInstance::ReadyActor(const OUTPUTDATA& _stData)
{
	// Add Default 
	{
		AddPrototype(PROTO_ACTOR_DEFAULTCUBE, CreateConstructorToNative<UDefaultCube>(
			_stData.wpDevice.lock(), LAYER_DEFAULT, CLONETYPE::CLONE_ONCE));

#ifdef _USE_DEBUGGING
		AddPrototype(PROTO_ACTOR_DEUBGGINGDEFAULTOBJECT, CreateConstructorToNative<UDefaultDebugging>(
			_stData.wpDevice.lock(), LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
		
		AddPrototype(PROTO_ACTOR_DEUBGGINGDEFAULTCELL, CreateConstructorToNative<UDefaultCell>(
			_stData.wpDevice.lock(), LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));

#endif 
	}
	AddPrototype(PROTO_ACTOR_PARTICLE, CreateConstructorToNative<UParticle>(
		_stData.wpDevice.lock(), LAYER_PARTICLE, CLONETYPE::CLONE_ONCE));

	AddPrototype(PROTO_ACTOR_ANIMATEPARTICLE, CreateConstructorToNative<UAnimatedParticle>(
		_stData.wpDevice.lock(), LAYER_PARTICLE, CLONETYPE::CLONE_ONCE));
	/*{
		AddPrototype(PROTO_ACTOR_TERRAIN, CreateConstructorToNative<UTerrain>(
			_stData.wpDevice.lock(), LAYER_TERRAIN, CLONETYPE::CLONE_ONCE));

		AddPrototype(PROTO_ACTOR_SKYBOX, CreateConstructorToNative<USkyBox>(
			_stData.wpDevice.lock(), LAYER_SKYBOX, CLONETYPE::CLONE_ONCE));

		AddPrototype(PROTO_ACTOR_PARTICLE, CreateConstructorToNative<UParticle>(
			_stData.wpDevice.lock(), LAYER_PARTICLE, CLONETYPE::CLONE_ONCE));

		AddPrototype(PROTO_ACTOR_ANIMATEPARTICLE, CreateConstructorToNative<UAnimatedParticle>(
			_stData.wpDevice.lock(), LAYER_PARTICLE, CLONETYPE::CLONE_ONCE));

		AddPrototype(PROTO_ACTOR_MIRROR, CreateConstructorToNative<UMirror>(
			_stData.wpDevice.lock(), LAYER_MIRROR, CLONETYPE::CLONE_ONCE));

		AddPrototype(PROTO_ACTOR_SCREENRENDEROBJ, CreateConstructorToNative<UScreenRenderObj>(
			_stData.wpDevice.lock(), LAYER_SCREENRENDEROBJ, CLONETYPE::CLONE_ONCE));

		AddPrototype(PROTO_RES_MIRRORCAMERA, CreateConstructorToNative<UMirrorCamera>(
			_stData.wpDevice.lock(), LAYTER_CAM, CLONETYPE::CLONE_ONCE));
	}*/
	return S_OK;
}

HRESULT UGameInstance::ReadyRenderTarget(const OUTPUTDATA& _stData)
{
	CSHPTRREF<GRAPHICDESC> GraphicDesc = m_spGraphicDevice->GetGraphicDesc();

	{
			// LightShadow 
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::LIGHTSHADE_AMBIENT_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } },
					RTDESC{ RTOBJID::LIGHTSHADE_SHADE_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
						GraphicDesc->iWinCX, GraphicDesc->iWinCY, {0.f, 0.f, 0.f, 1.f} },
					RTDESC{ RTOBJID::LIGHTSHADE_SPECULAR_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
							GraphicDesc->iWinCX, GraphicDesc->iWinCY, {0.f, 0.f, 0.f, 0.f } }
			};
			// Add RenderTargetGroup
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::LIGHTSHADE_DEFFERED, vecRts);
		}
		// NonAlpha_Deffered 
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::NONALPHA_DIFFUSE_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 1.f, 0.f } },
					RTDESC{ RTOBJID::NONALPHA_NOMRAL_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
						GraphicDesc->iWinCX, GraphicDesc->iWinCY, {1.f, 1.f, 1.f, 1.f}},
					RTDESC{ RTOBJID::NONALPHA_DEPTH_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
							GraphicDesc->iWinCX, GraphicDesc->iWinCY, {1.f, 1.f, 1.f, 1.f}},
					RTDESC{ RTOBJID::NONALPHA_POSITION_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
							GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } }
			};
			// Add RenderTargetGroup
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::NONALPHA_DEFFERED, vecRts);
		}
		// Alpha Deffered
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::ALPHA_DIFFUSE_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } },
					RTDESC{ RTOBJID::ALPHA_GLOW_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
						GraphicDesc->iWinCX, GraphicDesc->iWinCY, {0.f, 0.f, 0.f, 0.f} },
					RTDESC{ RTOBJID::ALPHA_GLOW_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT,
							GraphicDesc->iWinCX, GraphicDesc->iWinCY, {0.f, 0.f, 0.f, 0.f} }
			};
			// Add 
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::ALPHA_DEFFERED, vecRts);
		}
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::UI2D_SCREEN_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } }
			};
			// Add RenderTargetGroup
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::UI2D_DEFFERED, vecRts);
		}
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::BLEND_SCREEN_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } }
			};
			// Add RenderTargetGroup
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::BLEND_DEFFERED, vecRts);
		}


		// NonAlpha_Deffered 
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::SCREENNONA_DIFFUSE_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 1.f, 0.f } },
					RTDESC{ RTOBJID::SCREENNONA_NOMRAL_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
						GraphicDesc->iWinCX, GraphicDesc->iWinCY, {1.f, 1.f, 1.f, 1.f}},
					RTDESC{ RTOBJID::SCREENNONA_DEPTH_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
							GraphicDesc->iWinCX, GraphicDesc->iWinCY, {1.f, 1.f, 1.f, 1.f}},
					RTDESC{ RTOBJID::SCREENNONA_POSITION_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
							GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } }
			};
			// Add RenderTargetGroup
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::SCREEN_NONALPHA_DEFFERED, vecRts);
		}
		// Alpha Deffered
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::SCREENALPHA_DIFFUSE_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } },
					RTDESC{ RTOBJID::SCREENALPHA_GLOW_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
						GraphicDesc->iWinCX, GraphicDesc->iWinCY, {0.f, 0.f, 0.f, 0.f} },
					RTDESC{ RTOBJID::SCREENALPHA_GLOW_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT,
							GraphicDesc->iWinCX, GraphicDesc->iWinCY, {0.f, 0.f, 0.f, 0.f} }
			};
			// Add 
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::SCREEN_ALPHA_DEFFERED, vecRts);
		}
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::SCREENBLEND_SCREEN_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } }
			};
			// Add RenderTargetGroup
			m_spRenderTargetManager->AddRenderTargetGroup(RTGROUPID::SCREEN_BLEND_DEFFERED, vecRts);
		}
	}

#ifdef _USE_DEBUGGING
	/*
	렌더 타겟 디버깅을 위한 것
	*/
	m_spRenderTargetManager->AddDebugRenderObjects(RTGROUPID::NONALPHA_DEFFERED, RTOBJID::NONALPHA_NOMRAL_DEFFERED,
		_float2(100.f, 100.f), _float2(100.f, 100.f), m_spGraphicDevice->GetGraphicDesc());

	m_spRenderTargetManager->AddDebugRenderObjects(RTGROUPID::NONALPHA_DEFFERED, RTOBJID::NONALPHA_DIFFUSE_DEFFERED,
		_float2(100.f, 210.f), _float2(100.f, 100.f), m_spGraphicDevice->GetGraphicDesc());

	////m_spRenderTargetManager->AddDebugRenderObjects(RTGROUPID::NONALPHA_DEFFERED, RTOBJID::NONALPHA_NOMRAL_DEFFERED,
	////	_float2(100.f, 320.f), _float2(100.f, 100.f), m_spGraphicDevice->GetGraphicDesc());


	m_spRenderTargetManager->AddDebugRenderObjects(RTGROUPID::ALPHA_DEFFERED, RTOBJID::ALPHA_GLOW_DEFFERED,
		_float2(100.f, 430.f), _float2(100.f, 100.f), m_spGraphicDevice->GetGraphicDesc());

	//m_spRenderTargetManager->AddDebugRenderObjects(RTGROUPID::LIGHTSHADE_DEFFERED, RTOBJID::LIGHTSHADE_SHADE_DEFFERED,
	//	_float2(100.f, 540.f), _float2(100.f, 100.f), m_spGraphicDevice->GetGraphicDesc());

	//m_spRenderTargetManager->AddDebugRenderObjects(RTGROUPID::LIGHTSHADE_DEFFERED, RTOBJID::LIGHTSHADE_SPECULAR_DEFFERED,
	//	_float2(100.f, 650.f), _float2(100.f, 100.f), m_spGraphicDevice->GetGraphicDesc());
#endif
	return S_OK;
}
