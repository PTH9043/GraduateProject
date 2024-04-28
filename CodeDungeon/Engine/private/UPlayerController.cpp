#include "EngineDefine.h"
#include "UPlayerController.h"

UPlayerController::UPlayerController(CSHPTRREF<UDevice> _spDevice)
	: UController(_spDevice)
{
}

UPlayerController::UPlayerController(const UPlayerController& _rhs) :
	UController(_rhs)
{
}

void UPlayerController::Free()
{
}

HRESULT UPlayerController::NativeConstruct()
{
	return S_OK;
}

HRESULT UPlayerController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	return S_OK;
}

void UPlayerController::Tick(UCharacter* _pCharacter, const _double& _dTimeDelta)
{
}

_bool UPlayerController::InputTriggerData(UCharacter* _pCharacter, const _int _Data)
{
	return _bool();
}
