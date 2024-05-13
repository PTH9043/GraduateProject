#include "ClientDefines.h"
#include "CMummyAnimController.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"

CMummyAnimController::CMummyAnimController(CSHPTRREF<UDevice> _spDevice)
	: UAnimationController(_spDevice)
{
}

CMummyAnimController::CMummyAnimController(const CMummyAnimController& _rhs)
	: UAnimationController(_rhs)
{
}

void CMummyAnimController::Free()
{
}

HRESULT CMummyAnimController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CMummyAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

	m_wpMummyMob = std::dynamic_pointer_cast<CMummy>(GetOwnerCharacter());

	return S_OK;
}

void CMummyAnimController::Tick(const _double& _dTimeDelta)
{
	// Reset Trigger
	ClearTrigger();
	/* Reset */
	SetAnimState(-1);

	SHPTR< CMummy> spMummy = m_wpMummyMob.lock();
	SHPTR<UAnimModel> spAnimModel = spMummy->GetAnimModel();

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	_bool isIdle = false;
	_bool isMove = false;

	SetTrigger(L"IDLE");
	SetAnimState(ANIM_IDLE);

	spAnimModel->TickEvent(spMummy.get(), GetTrigger(), _dTimeDelta);

}
