#include "ServerDefines.h"
#include "CMummy.h"
#include "ATransform.h"
#include "CMummyAnimController.h"
#include "CMobLayoutLoader.h"
#include "ANavigation.h"
#include "ACoreInstance.h"
#include "ACell.h"

namespace Server
{
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer)
		: AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer), m_eMumyType{_MummyType}
	{
		SetMonsterType(TAG_CHAR::TAG_MUMMY);		
		UpdateFindRange(40.f,60.f);
	}
	_bool CMummy::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
		SetAnimController(Create<CMummyAnimController>(GetCoreInstance(), ThisShared<CMummy>(),
			"..\\..\\Resource\\Anim\\Mummy\\", "Mummy_DEMO_1_FBX.bin"));

		MOBDATA* pMoveData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		// Setting Animation 
		GetAnimController()->SetAnimation(pMoveData->strAnimName);
		GetTransform()->SetNewWorldMtx(pMoveData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		return true;
	}

	void CMummy::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AAnimation> spCurAnimation = GetAnimController()->GetCurAnimation();
		SHPTR<ATransform> spTransform = GetTransform();

		_int AnimIndex = GetAnimController()->GetCurAnimIndex();
		_double dDuration = GetAnimController()->GetElapsedTime();

		if (MOB_FIND == GetMonsterState())
		{

		}
		GetMonsterState();
		SC_MONSTERSTATE scMonsterState;
		PROTOFUNC::MakeScMonsterState(&scMonsterState, GetSessionID(), dDuration, AnimIndex, GetMonsterState());
		CombineProto<SC_MONSTERSTATE>(GetCopyBuffer(), GetPacketHead(), scMonsterState, TAG_SC_MONSTERSTATE);
		spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
	}

	void CMummy::Free()
	{
	}

}