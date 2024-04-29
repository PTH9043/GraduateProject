#include "EngineDefine.h"
#include "UPlayerController.h"


UPlayerController::UPlayerController(CSHPTRREF<UDevice> _spDevice, const _int _iKeyLength)
	: UController(_spDevice), m_iKeyLength{ _iKeyLength }
{
}

UPlayerController::UPlayerController(const UPlayerController& _rhs) :
	UController(_rhs), m_iKeyLength{_rhs.m_iKeyLength}
{
	m_PressingKeyContainer.resize(m_iKeyLength);
	m_UpKeyContainer.resize(m_iKeyLength);
	m_DownKeyContainer.resize(m_iKeyLength);
}

#pragma region STRUCT 

#pragma endregion STRUCT

void UPlayerController::Free()
{
}

HRESULT UPlayerController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UPlayerController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	if (FAILED(__super::NativeConstructClone(_tDatas)))
		return E_FAIL;



	return S_OK;
}

void UPlayerController::Tick(UCharacter* _pCharacter, const _double& _dTimeDelta)
{
}

_bool UPlayerController::InputTriggerData(UCharacter* _pCharacter, const _int _Data)
{
	return _bool();
}

void UPlayerController::CheckInput(const INPUTDESC _InputDesc)
{
}
