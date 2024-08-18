#include "ClientDefines.h"
#include "CNetworkWarriorPlayerController.h"
#include "CWarriorPlayer.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UProcessedData.h"
#include "UMethod.h"

CNetworkWarriorPlayerController::CNetworkWarriorPlayerController(CSHPTRREF<UDevice> _spDevice)
	: UAnimationController(_spDevice), m_JumpSpeed{0}, m_dRecvAnimDuration{0}
{
}

CNetworkWarriorPlayerController::CNetworkWarriorPlayerController(const CNetworkWarriorPlayerController& _rhs) :
	UAnimationController(_rhs), m_dRecvAnimDuration{ 0 }
{
}

void CNetworkWarriorPlayerController::Free()
{
}

HRESULT CNetworkWarriorPlayerController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CNetworkWarriorPlayerController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SHPTR<UCharacter> spCharacter = GetOwnerCharacter();
	m_wpWarriorPlayer = std::dynamic_pointer_cast<CWarriorPlayer>(spCharacter);
	spCharacter->GetAnimModel()->NotApplyAnimPositionEnable();

	return S_OK;
}

void CNetworkWarriorPlayerController::Tick(const _double& _dTimeDelta)
{
	SHPTR<CWarriorPlayer> spWarriorPlayer = m_wpWarriorPlayer.lock();
	SHPTR<UAnimModel> spAnimModel = spWarriorPlayer->GetAnimModel();
	// Jump or fall state
	if (spWarriorPlayer->GetJumpingState() || spWarriorPlayer->GetFallingState()) {
		spWarriorPlayer->GetTransform()->MoveForward(_dTimeDelta, static_cast<_float>(m_JumpSpeed));
	}
	_wstring CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();
	// Combo attack state
	if (CurAnimName == L"attack04" || CurAnimName == L"combo09" || CurAnimName == L"combo05" || CurAnimName == L"combo06_1" || CurAnimName == L"combo06_2" || CurAnimName == L"combo06_3" || CurAnimName == L"combo02_1" || CurAnimName == L"combo02_2" || CurAnimName == L"combo02_3") {
		spWarriorPlayer->IfAttack(true);
	}
	else {
		spWarriorPlayer->IfAttack(false);
	}

	spAnimModel->TickAnimation(_dTimeDelta);
	spAnimModel->TickEvent(spWarriorPlayer.get(), GetTrigger(), _dTimeDelta);
}

void CNetworkWarriorPlayerController::ReceiveNetworkProcessData(void* _pData)
{
#ifdef _ENABLE_PROTOBUFF
	SHPTR<CWarriorPlayer> spWarriorPlayer = m_wpWarriorPlayer.lock();
	SHPTR<UAnimModel> spAnimModel = spWarriorPlayer->GetAnimModel();
	{
		CHARSTATE* pPlayerData = static_cast<CHARSTATE*>(_pData);
		SetAnimState(pPlayerData->state());

		if (pPlayerData->animationindex() != spAnimModel->GetCurrentAnimIndex())
			spAnimModel->SetAnimation(pPlayerData->animationindex());
	}
#endif
}
