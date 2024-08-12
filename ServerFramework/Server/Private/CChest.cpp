#include "ServerDefines.h"
#include "CChest.h"
#include "ATransform.h"
#include "AAnimController.h"
#include "CChestAnimController.h"
#include "ACoreInstance.h"
#include "ASession.h"

namespace Server {
	CChest::CChest(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) : 
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_CHEST);
		UpdateFindRange(10.f, 20.f);
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
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
	}

	void CChest::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);

		if (true == IsCurrentFindPlayer())
		{
			SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

			static SC_MONSTERFIND scMonsterFind;
			PROTOFUNC::MakeScMonsterFind(&scMonsterFind, GetSessionID(), _spSession->GetSessionID());
			CombineProto<SC_MONSTERFIND>(GetCopyBuffer(), GetPacketHead(), scMonsterFind, TAG_SC_MONSTERFIND);
			spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
		}
	}

	void CChest::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CChest::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	//	__super::Collision(_pGameObject, _dTimeDelta);
	}

	void CChest::LastBehavior()
	{
		SHPTR<ASession> spSession = GetTargetSession();
		spSession->HealHp(210.f);
	}

	void CChest::Free()
	{
	}

}