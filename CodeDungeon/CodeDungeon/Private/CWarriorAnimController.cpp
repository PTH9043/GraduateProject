#include "ClientDefines.h"
#include "CWarriorAnimController.h"
#include "CWarriorPlayer.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"

CWarriorAnimController::CWarriorAnimController(CSHPTRREF<UDevice> _spDevice) 
	: UAnimationController(_spDevice)
{
}

CWarriorAnimController::CWarriorAnimController(const CWarriorAnimController& _rhs) : 
	UAnimationController(_rhs)
{
}

void CWarriorAnimController::Free()
{
}

HRESULT CWarriorAnimController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CWarriorAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

	m_wpWarriorPlayer = std::dynamic_pointer_cast<CWarriorPlayer>(GetOwnerCharacter());

	return S_OK;
}

void CWarriorAnimController::Tick(const _double& _dTimeDelta)
{
	ClearTrigger();
	/* Reset */
	SetAnimState(-1);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	_bool isIdle = false;
	_bool isMove = false;
	{
		_bool isW = spGameInstance->GetDIKeyPressing(DIK_W);
		_bool isA = spGameInstance->GetDIKeyPressing(DIK_A);
		_bool isS = spGameInstance->GetDIKeyPressing(DIK_S);
		_bool isD = spGameInstance->GetDIKeyPressing(DIK_D);

		isMove = isW || isA || isS || isD;
	}

	_bool isAttack = spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_L);

	isIdle = !isMove && !isAttack;
	if (true == isIdle)
	{
		SetTrigger(L"IDLE");
		SetAnimState(ANIM_IDLE);
		return;
	}

	if (true == isMove)
	{
		SetTrigger(L"WALKF");
		SetAnimState(ANIM_MOVE);
		return;
	}

	if (true == isAttack)
	{
		SetTrigger(L"ATTACK01");
		SetAnimState(ANIM_ATTACK);
		return;
	}
}
