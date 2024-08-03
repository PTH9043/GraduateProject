#include "ServerDefines.h"
#include "CChest.h"
#include "ATransform.h"
#include "AAnimController.h"
#include "CChestAnimController.h"

namespace Server {
	CChest::CChest(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) : 
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_CHEST);
		UpdateFindRange(40.f, 90.f);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
	}

	_bool CChest::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f);
		SetAnimController(Create<CChestAnimController>(GetCoreInstance(), ThisShared<CChest>(),
			"..\\..\\Resource\\Anim\\Chest\\", "Chest 1_FBX.bin", Matrix));
		return __super::Start(_ReceiveDatas);
	}

	void CChest::Tick(const _double& _dTimeDelta)
	{
	}

	void CChest::LateTick(const _double& _dTimeDelta)
	{
	}

	void CChest::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	//	__super::State(_spSession, _MonsterState);
	}

	void CChest::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	bool CChest::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CChest::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CChest::Free()
	{
	}

}