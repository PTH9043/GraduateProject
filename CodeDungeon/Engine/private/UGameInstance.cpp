#include "EngineDefine.h"
#include "UGameInstance.h"

#include "UResourceManager.h"
#include "UGraphicDevice.h"

IMPLEMENT_SINGLETON(UGameInstance);

UGameInstance::UGameInstance() :
	m_spResourceManager(Create<UResourceManager>())
{

}

UGameInstance::~UGameInstance()
{
	//Free();
}

void UGameInstance::Free()
{
	//ClearOnceTypeData();
	m_spResourceManager.reset();


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
