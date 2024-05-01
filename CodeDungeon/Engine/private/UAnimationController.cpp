#include "EngineDefine.h"
#include "UAnimationController.h"
#include "UCharacter.h"

UAnimationController::UAnimationController(CSHPTRREF<UDevice> _spDevice)
	: UController(_spDevice), m_iAnimState{0}
{
}

UAnimationController::UAnimationController(const UAnimationController& _rhs) :
	UController(_rhs), m_iAnimState{0}
{
}

void UAnimationController::Free()
{
}

HRESULT UAnimationController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UAnimationController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);
	return S_OK;
}

void UAnimationController::Tick(const _double& _dTimeDelta)
{

}

