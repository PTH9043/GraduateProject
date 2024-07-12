#include "ServerDefines.h"
#include "CChest.h"
#include "ATransform.h"
#include "AAnimController.h"
#include "CChestAnimController.h"

namespace Server {
	CChest::CChest(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) : 
		AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_MUMMY);
		UpdateFindRange(40.f, 60.f);
		SetMoveSpeed(10);
		SetAttackRange(20.f);
		SetActive(false);
	}

	_bool CChest::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
		SetAnimController(Create<CChestAnimController>(GetCoreInstance(), ThisShared<CChest>(),
			"..\\..\\Resource\\Anim\\Chest\\", "Chest_FBX.bin"));

		MOBDATA* pMoveData = static_cast<MOBDATA*>(_ReceiveDatas[0]);

		// Setting Animation 
		GetAnimController()->SetAnimation(pMoveData->strAnimName);
		GetTransform()->SetPos(pMoveData->mWorldMatrix.Get_Pos());
		GetTransform()->SetDirection(pMoveData->mWorldMatrix.Get_Look());
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		return true;
	}

	void CChest::Tick(const _double& _dTimeDelta)
	{
	}

	void CChest::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	}

	void CChest::Free()
	{
	}

}