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
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID, MUMMYTYPE _MummyType)
		: AMonster(OBJCON_CONDATA, _ID), m_eMumyType{_MummyType}
	{
		SetMonsterType(TAG_CHAR::TAG_MUMMY);
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
	}

	void CMummy::Free()
	{
	}

}