#include "ClientDefines.h"
#include "CWarriorAnimController.h"
#include "CWarriorPlayer.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"

CWarriorAnimController::CWarriorAnimController(CSHPTRREF<UDevice> _spDevice) 
	: UAnimationController(_spDevice), m_isComboStack{false}
{
}

CWarriorAnimController::CWarriorAnimController(const CWarriorAnimController& _rhs) : 
	UAnimationController(_rhs), m_isComboStack{ false }
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
	// Reset Trigger
	ClearTrigger();

	SetAnimState(-1);

	SHPTR< CWarriorPlayer> spWarriorPlayer = m_wpWarriorPlayer.lock();
	SHPTR<UAnimModel> spAnimModel = spWarriorPlayer->GetAnimModel();

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	_bool isIdle = false;
	_bool isRunshift = spGameInstance->GetDIKeyPressing(DIK_LSHIFT);
	_bool isMoveFront = false;
	{
		_bool isW = spGameInstance->GetDIKeyPressing(DIK_W);
		_bool isA = spGameInstance->GetDIKeyPressing(DIK_A);
		_bool isS = spGameInstance->GetDIKeyPressing(DIK_D);

		isMoveFront = isW || isA || isS;
	}

	_bool isMoveBack = spGameInstance->GetDIKeyPressing(DIK_S);

	_bool isAttack = spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_L);
	_bool isCombo = spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_R);

	if (false == isMoveFront && false == isMoveBack && false == isAttack && false == isCombo)
	{
		UpdateState(spAnimModel, ANIM_IDLE, L"IDLE");
		m_isComboStack = false;
	}
	else if (ANIM_ATTACK == GetAnimState())
	{
		UpdateState(spAnimModel, ANIM_IDLE, L"IDLE");
		return;
	}

	if (true == isMoveFront)
	{
		if (false == isRunshift)
		{
			UpdateState(spAnimModel, ANIM_MOVE, L"WALKF");
		}
		else
		{
			UpdateState(spAnimModel, ANIM_RUN, L"RUNF");
		}
		m_isComboStack = false;
	}

	if (true == isMoveBack)
	{
		spAnimModel->ResetCurAnimEvent();
		if (false == isRunshift)
		{
			UpdateState(spAnimModel, ANIM_WALKBACK, L"WALKB");
		}
		else
		{
			UpdateState(spAnimModel, ANIM_RUNBACK, L"RUNB");
		}
		m_isComboStack = false;
	}

	if (true == isAttack)
	{
		_int iRan = rand() % 3;
		if (0 == iRan)
		{
			UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK01");
		}
		else if (1 == iRan)
		{
			UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK02");
		}
		else  
		{
			UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK04");
		}

		m_isComboStack = false;
	}

	if (true == isCombo)
	{
		spAnimModel->ResetCurAnimEvent();
		if (true == m_isComboStack)
		{
			UpdateState(spAnimModel, ANIM_COMBO, L"NEXT");
		}
		else
		{
			UpdateState(spAnimModel, ANIM_COMBO, L"COMBO04");
			m_isComboStack = true;
		}
	}

	spAnimModel->TickEvent(spWarriorPlayer.get(), GetTrigger(), _dTimeDelta);
}
