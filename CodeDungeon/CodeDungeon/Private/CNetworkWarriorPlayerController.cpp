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
	: UAnimationController(_spDevice), m_JumpSpeed{0}
{
}

CNetworkWarriorPlayerController::CNetworkWarriorPlayerController(const CNetworkWarriorPlayerController& _rhs) :
	UAnimationController(_rhs)
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

	return S_OK;
}

void CNetworkWarriorPlayerController::Tick(const _double& _dTimeDelta)
{

}

void CNetworkWarriorPlayerController::ReceiveNetworkProcessData(void* _pData)
{
#ifdef _ENABLE_PROTOBUFF

	SC_PLAYERSTATE* pPlayerData = reinterpret_cast<SC_PLAYERSTATE*>(_pData);

	SetTrigger(UMethod::ConvertSToW(pPlayerData->triggername()));
	SetAnimState(pPlayerData->animstate());

#endif
}
