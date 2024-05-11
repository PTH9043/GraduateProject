#include "EngineDefine.h"
#include "ULight.h"
#include "UGameInstance.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"
#include "UVIBufferRect.h"

#include "URenderTargetManager.h"
//#include "URenderObject.h"

ULight::ULight(CSHPTRREF<UDevice> _spDevice, const LIGHTINFO& _stLightInfo, const LIGHTCONTROL& _stLightControl)
	: UObject(_spDevice),
	m_spLightConstantBuffer{ nullptr },
	m_stLightParam{ _stLightInfo, _stLightControl }
{
}

void ULight::Free()
{
}

HRESULT ULight::NativeConstruct()
{
	m_isActive = true;
	m_spLightConstantBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::LIGHTCONTROL, GetTypeSize<LIGHTCONTROLPARAM>());
	return S_OK;
}

void ULight::Render(SHPTR<UCommand> spCmdList,CSHPTRREF<UVIBufferRect> _spVIBufferRect, CSHPTRREF<UShader> _spShader)
{
	RETURN_CHECK(nullptr == _spShader || nullptr == _spVIBufferRect, ;);
	_spShader->BindCBVBuffer(m_spLightConstantBuffer, &m_stLightParam, GetTypeSize<LIGHTCONTROLPARAM>());
	_spVIBufferRect->Render(_spShader, spCmdList);
}


void ULight::OutLightInfo(LIGHTINFO& _stLightInfo)
{
	if (true == m_isActive) {
		_stLightInfo = m_stLightParam.tLightInfo;
	}
}

void ULight::OutLightControl(LIGHTCONTROL& _stLightControl)
{
}
