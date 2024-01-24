#pragma once
#include "EngineDefine.h"
#include "UBase.h"

BEGIN(Engine)
class UResource;
class UCommand;
class UTexture;
class UDevice;
class UGpuCommand;
class UTableDescriptor;
class URootSignature;

class UResourceManager;
class UGraphicDevice;

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
private:
	SHPTR<UResourceManager>				m_spResourceManager;
	SHPTR<UGraphicDevice>						m_spGraphicDevice;
};

END