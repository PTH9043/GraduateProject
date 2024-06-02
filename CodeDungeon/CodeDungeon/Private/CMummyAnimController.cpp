#include "ClientDefines.h"
#include "CMummyAnimController.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"

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
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	// Reset Trigger
	ClearTrigger();
	/* Reset */
	SetAnimState(-1);

	static _bool AttackMode = false;
	static _bool TauntMode = false;

	SHPTR< CMummy> spMummy = m_wpMummyMob.lock();
	SHPTR<UAnimModel> spAnimModel = spMummy->GetAnimModel();
	const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

	_float DistanceFromPlayer = spMummy->GetDistanceFromPlayer();
	_bool FoundPlayer = spMummy->GetFoundTargetState();
	_bool idle = true;

	if (FoundPlayer && !AttackMode && !TauntMode)
	{
		UpdateState(spAnimModel, ANIM_AWAKE, L"WAKEUP");
	}

	if (CurAnimName == L"openLaying" || CurAnimName == L"openStanding")
		TauntMode = true;

	if (TauntMode)
	{
		UpdateState(spAnimModel, ANIM_TAUNT, L"TAUNT");
	}

	if (CurAnimName == L"taunt")
		AttackMode = true;

	if (AttackMode)
	{
		if(idle)
			UpdateState(spAnimModel, ANIM_IDLE, L"IDLE");
	}



	spAnimModel->TickEvent(spMummy.get(), GetTrigger(), _dTimeDelta);

}
