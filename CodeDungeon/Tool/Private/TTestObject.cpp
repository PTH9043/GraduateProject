#include "ToolDefines.h"
#include "TTestObject.h"
#include "UGameInstance.h"
#include "UVIBufferRect.h"
#include "UTransform.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"

TTestObject::TTestObject(CSHPTRREF<UDevice> _spDevice, 
	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::STATIC),
	m_spVIBufferRect{ nullptr }, m_spRectColorBuffer{nullptr}
{
}

TTestObject::TTestObject(const TTestObject& _rhs) : 
	UPawn(_rhs), m_spVIBufferRect{ nullptr }
{
}

void TTestObject::Free()
{
}

HRESULT TTestObject::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TTestObject::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spVIBufferRect = static_pointer_cast<UVIBufferRect>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERRECT));
	// Add Shader 
	AddShader(PROTO_RES_RECTSHADER);

	m_spRectColorBuffer = CreateNative< UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::B3, RECTCOLOR_SIZE, 1);
	m_RectColor.g_RectColor = { 1.f, 1.f, 0.f, 1.f };
	return S_OK;
}

void TTestObject::TickActive(const _double& _dTimeDelta)
{
}

void TTestObject::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);
}

HRESULT TTestObject::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetTransform()->BindTransformData(GetShader());
	GetShader()->BindCBVBuffer(m_spRectColorBuffer, &m_RectColor, RECTCOLOR_SIZE);
	m_spVIBufferRect->Render(GetShader(), _spCommand);
	return S_OK;
}

void TTestObject::Collision(CSHPTRREF<UPawn> _pEnemy)
{

}