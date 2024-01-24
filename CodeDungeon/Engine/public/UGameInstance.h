#pragma once
#include "UBase.h"

BEGIN(Engine)
class UResource;
class UCommand;
class UTexture;
class UDevice;
class UGpuCommand;
class UTableDescriptor;
class URootSignature;
class USwapChain;
class UGlobalConstantBuffer;
class UCamera;
class UActor;

class UResourceManager;
class UGraphicDevice;
class URenderTargetManager;
class URenderTargetGroup;
class UShaderBufferManager;
class UPipeLine;
class UActorManager;


using CLONES = UNORMAP<_wstring, SHPTR<UActorGroup>>;
using CLONEARR = ARRAY<CLONES, CLONE_MAX_SIZE>;

class UGameInstance : public UBase 
{
	DECLARE_SINGLETON(UGameInstance)
public:
	UGameInstance();
	virtual ~UGameInstance();
public:
	/* GameInstance */
	// Free 
	virtual void Free() override;
	// Ready Instance
	HRESULT ReadyInstance(const GRAPHICDESC& _stDesc, OUTPUTDATA& _stOutDesc);

public: /* ResourceManager*/
	HRESULT AddPrototype(const _wstring& _wstrPrototype, const CLONETYPE& _eCloneType, CSHPTRREF<UResource> _spResource);
	SHPTR<UResource> CloneResource(const _wstring& _wstrPrototype, const VOIDDATAS& _stDatas = VOIDDATAS{});
	void		FindResourceProtoTag(CSHPTRREF<UResource> _spResource, _wstring& _wstrResourceTag);

	void AddPipeLineState(const _wstring& _wstrName, const ComPtr<Dx12PipelineState>& _cpPipeLineState);
	void SettingPipeLineState(const _wstring& _wstrName, CSHPTRREF<UCommand> _spCommand);

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
	CSHPTRREF<UDevice>			GetDevice() const;
	CSHPTRREF<UGpuCommand>		GetGpuCommand() const;
	CSHPTRREF<UTableDescriptor>	GetTableDescriptor() const;
	CSHPTRREF<URootSignature>	GetRootSignature() const;

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

public: /* ShaderBufferManager*/
	void	 BindGlobalBuffer(const GLOBAL_CBUFFERTYPE _eGrobalCBuffer, CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize);
	HRESULT GetGlobalConstantBuffer(const GLOBAL_CBUFFERTYPE _eGrobalCBuffer, SHPTRREF<UGlobalConstantBuffer> _spGrobalConstantBuffer);
	HRESULT GetPreAllocatedConstantBuffer(const PREALLOCATED_CBUFFERTYPE _ePreAllocatedCBufferType, SHPTRREF<UShaderConstantBuffer> _spShaderConstantBuffer);

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

public: /* ActorManager */
	const  CLONEARR& GetClonesArr() const;
	HRESULT AddPrototype(const _wstring& _wstrName, CSHPTRREF<UActor> _spActor);
	void  CloneActor(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAdd(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAddAndNotInLayer(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	void RemoveActor(CSHPTRREF<UActor> _spActor);

private:
	SHPTR<UResourceManager>				m_spResourceManager;
	SHPTR<UGraphicDevice>						m_spGraphicDevice;
	SHPTR<URenderTargetManager>		m_spRenderTargetManager;
	SHPTR<UShaderBufferManager>		m_spShaderBufferManager;
	SHPTR<UPipeLine>								m_spPipeLine;
	SHPTR<UActorManager>						m_spActorManager;
};

END