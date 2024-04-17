#include "EngineDefine.h"
#include "UDefaultCell.h"
#include "UVIBufferCell.h"
#include "UTransform.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"

UDefaultCell::UDefaultCell(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::NON),
	m_spVIBuffer{ nullptr }, m_spDebuggingConstantBuffer{ nullptr }
{
}

UDefaultCell::UDefaultCell(const UDefaultCell& _rhs)
	: UPawn(_rhs),
	m_spVIBuffer{ nullptr }, m_spDebuggingConstantBuffer{ nullptr }
{
}

void UDefaultCell::SetVIBuffer(ARRAY<_float3, 3>& _Points)
{
	m_spVIBuffer.reset();
	m_spVIBuffer = CreateConstructorNative<UVIBufferCell>(GetDevice(), _Points);
}

void UDefaultCell::Free()
{

}

HRESULT UDefaultCell::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UDefaultCell::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);

	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	m_spVIBuffer = UMethod::ConvertTemplate_Index<SHPTR<UVIBufferCell>>(_vecDatas, 0);
	
	AddShader(PROTO_RES_DEBUGGINGWIREFRAMESHADER);

	m_spDebuggingConstantBuffer = CreateNative< UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::B14, DEBUGPARAM_SIZE, 1);
	m_stDebuggParam.g_DebugColor = { 0.f, 1.f, 0.f, 1.f };

	return S_OK;
}

void UDefaultCell::AddRenderer(RENDERID _eRenderID)
{
	AddRenderGroup(_eRenderID);
}

void UDefaultCell::TickActive(const _double& _dTimeDelta)
{
}

void UDefaultCell::LateTickActive(const _double& _dTimeDelta)
{ 
}

HRESULT UDefaultCell::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	// Settings 
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetShader()->BindCBVBuffer(m_spDebuggingConstantBuffer, &m_stDebuggParam, DEBUGPARAM_SIZE);
	GetTransform()->BindTransformData(GetShader());
	m_spVIBuffer->Render(GetShader(), _spCommand);
	return S_OK;
}

void UDefaultCell::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
