#include "ClientDefines.h"
#include "CWarriorAnimController.h"
#include "CWarriorPlayer.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"

CWarriorAnimController::CWarriorAnimController(CSHPTRREF<UDevice> _spDevice) 
	: UAnimationController(_spDevice), m_wstrTrigger{L""}
{
}

CWarriorAnimController::CWarriorAnimController(const CWarriorAnimController& _rhs) : 
	UAnimationController(_rhs), m_wstrTrigger{ L"" }
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
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<UAnimModel> spAnimModel = GetOwnerCharacter()->GetAnimModel();

	_bool isIdle = false;
	_bool isMove = false;
	{
		_bool isW = spGameInstance->GetDIKeyPressing(DIK_W);
		_bool isA = spGameInstance->GetDIKeyPressing(DIK_A);
		_bool isS = spGameInstance->GetDIKeyPressing(DIK_S);
		_bool isD = spGameInstance->GetDIKeyPressing(DIK_D);

		isMove = isW || isA || isS || isD;
	}

	if (true == isMove)
	{
		m_wstrTrigger = L"MOVE";
		SetAnimState(ANIM_MOVE);
	}

	isIdle = !isMove;
	if (true == isIdle)
		m_wstrTrigger = L"IDLE";
	// TickEvent
	spAnimModel->TickEvent(GetOwnerCharacter().get(), m_wstrTrigger, _dTimeDelta);
	SetAnimState(ANIM_IDLE);
}
