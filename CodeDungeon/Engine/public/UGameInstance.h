#pragma once
#include "UBase.h"

BEGIN(Engine)
class UGraphicRenderManager;
class UShaderBufferManager;
class UGraphicDevice;
class UTimerManager;
class UInputManager;

class UFontManager;
class UActorManager;
class UComponentManager;
class UResourceManager;
class USceneManager;

class UPipeLine;
class URenderTargetManager;
class UComputeManager;
class UPicking;
class UFilePathManager;
class URandomManager;

class URenderer;

class URootSignature;
class UShaderConstantBuffer;
class UGlobalConstantBuffer;
class UShader;
class UTexture;
class URenderTargetGroup;

class UActor;
class UComponent;
class UResource;
class UCamera;
class UTimer;
class UVIBuffer;
class UScene;
class ULight;
class UTableDescriptor;
class UGpuCommand;

class UActorGroup;
class UCommand;
class UFont;

struct PICKINGDESC;
struct WAITCHECKACTOR;

using CLONES = UNORMAP<_wstring, SHPTR<UActorGroup>>;
using CLONEARR = ARRAY<CLONES, CLONE_MAX_SIZE>;

class UGameInstance : public UBase 
{
	DECLARE_SINGLETON(UGameInstance)
public:
	UGameInstance();
	virtual ~UGameInstance();
public:
	const _bool IsGamming() const { return m_isGamming; }
	void SetGamming(const _bool _isGamming) { this->m_isGamming = _isGamming; }

public:
	/* GameInstance */
	// Free 
	virtual void Free() override;
	// Ready Instance
	HRESULT ReadyInstance(const GRAPHICDESC& _stDesc, OUTPUTDATA& _stOutDesc);
	// Create Shader
	HRESULT CreateGraphicsShader(const _wstring& _wstrProtoName, const CLONETYPE _eCloneType,
		const SHADERDESC& _stShaderDesc, const GRAPHICRENDEROBJECT_TYPE _eType = GRAPHICRENDEROBJECT_TYPE::COMMON);
public:
	void OtherFrame(const _double& _dTimeDelta, const WPARAM& _wParam);
	void AwakeTick();
	void Tick(const _double& _dTimeDelta);
	void LateTick(const _double& _dTimeDelta);
	void RenderBegin();
	void RenderEnd();
public:
	HRESULT	OnWindowResize(const _uint& _iWinSizeX, const _uint& _iWinSizeY, const GRAPHICDESC::WINMODE _eWindowMode);
	// Clear Once Type data 
	void ClearOnceTypeData();
public:
	CSHPTRREF<UDevice>			GetDevice() const;
	CSHPTRREF<UGpuCommand>		GetGpuCommand() const;
	CSHPTRREF<UTableDescriptor>	GetTableDescriptor() const;
	CSHPTRREF<URootSignature>	GetRootSignature() const;

public: /*  GraphicDevice */
	const _bool	IsGamePaused() const;
	const _bool	IsScreenMinimized() const;
	const _bool	IsScreenMaximized() const;
	const _bool	IsScreenFullScreen() const;
	const _bool	IsMssa4xState() const;
	const _uint& GetMssa4xQuality() const;
	const D3D12_VIEWPORT& GetD3DViewport() const;
	const D3D12_RECT& GetD3DWindowSizeRect() const;
	CSHPTRREF<GRAPHICDESC> GetGraphicDesc() const;
	// Ready
	SHPTR<UTexture>		GetDefaultBackTexture() const;

public: /* TimerManager */
	HRESULT CreateTimer(const _wstring& _wstrName);
	SHPTR<UTimer> CreateTimerAdd(const _wstring& _wstrName);
	HRESULT RemoveTimer(const _wstring& _wstrName);

public: /* ActorManager */
	const  CLONEARR& GetClonesArr() const;
	HRESULT AddPrototype(const _wstring& _wstrName, CSHPTRREF<UActor> _spActor);
	void  CloneActor(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAdd(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAddAndNotInLayer(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	void RemoveActor(CSHPTRREF<UActor> _spActor);

public: /*  ComponentManager */
	HRESULT AddPrototype(const _wstring& _wstrPrototypes, CSHPTRREF<UComponent> _spComponent);
	SHPTR<UComponent> CloneComp(const _wstring& _wstrPrototypes, const VOIDDATAS& _stDatas = VOIDDATAS{});

public: /* ResourceManager*/
	HRESULT AddPrototype(const _wstring& _wstrPrototype, const CLONETYPE& _eCloneType, CSHPTRREF<UResource> _spResource);
	SHPTR<UResource> CloneResource(const _wstring& _wstrPrototype, const VOIDDATAS& _stDatas = VOIDDATAS{});
	void		FindResourceProtoTag(CSHPTRREF<UResource> _spResource, _wstring& _wstrResourceTag);

	void AddPipeLineState(const _wstring& _wstrName, const ComPtr<Dx12PipelineState>& _cpPipeLineState);
	void SettingPipeLineState(const _wstring& _wstrName, CSHPTRREF<UCommand> _spCommand);

public: /* SceneManager*/
	void OutLightControlInfo(LIGHTPARAM& _stLightControl);
	void ChangeLightCamIndex(const CAMID& _iID);

	CSHPTRREF<UScene> GetCurScene() const;
	void RegisterScene(CSHPTRREF<UScene> _spScene);
	// Add
	HRESULT AddLight(const LIGHTINFO& _stInfo);
	// Out Light
	void OutLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex, SHPTR<ULight>& _spLight);
	// Active
	HRESULT ActiveLIght(const LIGHTTYPE& _eLightType, const _uint& _iIndex, const _bool& _isActive);
	// Delete
	HRESULT DeleteLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex);
	// Clear
	HRESULT ClearLight();



public: /*  RenderTargetManager */
	HRESULT		AddRenderTargetGroup(const RTGROUPID& _eGroupID, const std::vector<RTDESC>& _rtVec);
	void					RemoveRenderTargetGroup(const RTGROUPID _eGroupID);

	// OMSetRenderTarget 
	void					OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index, const _uint _iOffset);
	void					OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	// ClearRenderTargetView
	void					ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index);
	void					ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	// Waiting
	void					WaitTargetToResource(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	void					WaitResourceToTarget(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	// Get RT Group 
	SHPTR<URenderTargetGroup>	 FindRenderTargetGroup(const RTGROUPID& _eGroupID);
	SHPTR<UTexture> FindRenderTargetTexture(const RTGROUPID _eGroupID, const RTOBJID _eObjID);

public: /* PipeLine */
	void RegisterCameraInPipeline(CSHPTRREF<UCamera> _spCamera, CAMID& _iCamID,
		const VIEWPROJMATRIX& _stViewProjMatrix, const CAMERATYPE _eType = CAMERATYPE::OTHER);
	void RemoveCameraInPipeLine(CAMID _CamID);

	_bool IsFrustomContains(const _float3& _vPos, const _float _fRadius, const CAMID& _iCamID = MAIN_CAMERA_ID);

	const _float4x4 GetMainCamViewMatrix() const;
	const _float4x4& GetMainCamProjMatrix() const;
	const _float3& GetMainCamPosition();

	const _float4x4 GetCamViewMatrix(const CAMID& _iID) const;
	const _float4x4& GetCamProjMatrix(const CAMID& _iID) const;
	const _float3& GetCameraPosition(const CAMID& _iID);
	const _float GetCamFar(const CAMID& _iID);

	void ChangeRenderCamera(CSHPTRREF<UCamera> _spCamera);
	void ChangeRenderCamera(CAMID _iID);

	void AddRenderCamList(CSHPTRREF<UCamera> _spCamera);
	void AddRenderCamList(CAMID _iID);

	const CAMID GetRenderCamID() const;

public: /* FilePath Manager*/
	SHPTR<FILEGROUP> FindFolder(const PATHS& _vecFolderList);
	SHPTR<FILEGROUP>	FindFolder(const _wstring& _wstrFindName, const _wstring& _wstrParentsFolderName = L"");

	HRESULT LoadFirstFilder(const _wstring& _wstrFilePath);


public: /* ShaderBufferManager*/
	void	 BindGlobalBuffer(const GLOBAL_CBUFFERTYPE _eGrobalCBuffer, CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize);
	HRESULT GetGlobalConstantBuffer(const GLOBAL_CBUFFERTYPE _eGrobalCBuffer, SHPTRREF<UGlobalConstantBuffer> _spGrobalConstantBuffer);
	HRESULT GetPreAllocatedConstantBuffer(const PREALLOCATED_CBUFFERTYPE _ePreAllocatedCBufferType, SHPTRREF<UShaderConstantBuffer> _spShaderConstantBuffer);


private: /* Ready Datas */
	HRESULT ReadyResource(const OUTPUTDATA& _stData);
	HRESULT ReadyComp(const OUTPUTDATA& _stData);
	HRESULT ReadyActor(const OUTPUTDATA& _stData);
	HRESULT ReadyRenderTarget(const OUTPUTDATA& _stData);
private:

	_bool															m_isGamming;
	
	SHPTR<UResourceManager>				m_spResourceManager;
	SHPTR<UGraphicDevice>						m_spGraphicDevice;
	SHPTR<URenderTargetManager>		m_spRenderTargetManager;
	SHPTR<UShaderBufferManager>		m_spShaderBufferManager;
	SHPTR<UPipeLine>								m_spPipeLine;
	SHPTR<UActorManager>						m_spActorManager;

	SHPTR<UTimerManager>						m_spTimerManager;
	SHPTR<UComponentManager>			m_spComponentManager;
	SHPTR<USceneManager>					m_spSceneManager;
	SHPTR<URenderer>								m_spRenderer;
	SHPTR<UFilePathManager>					m_spFilePathManager;
	
	
	//SHPTR<UGraphicRenderManager>	m_spGraphicRenderManager;
	//SHPTR<UInputManager>						m_spInputManager;
	//SHPTR< UFontManager>						m_spFontMananger;
	//SHPTR<UComputeManager>				m_spComputeManager;
	//SHPTR<UPicking>									m_spPicking;

	//SHPTR<URandomManager>				m_spRandomManager;
	


	

	
	
	

	
	

};

END