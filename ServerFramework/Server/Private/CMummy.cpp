#include "ServerDefines.h"
#include "CMummy.h"
#include "ATransform.h"
#include "CMummyAnimController.h"
#include "CMobLayoutLoader.h"
#include "ANavigation.h"
#include "ACoreInstance.h"
#include "ACell.h"
#include "AAnimator.h"
#include "AAnimController.h"
#include "AAnimation.h"
#include "ASession.h"

namespace Server
{
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer)
		: CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer),
		m_eMummyType{ _MummyType }
	{
		if (MUMMY_LAYING == m_eMummyType)
		{
			SetMonsterType(TAG_CHAR::TAG_MUMMY_LAYING);
		}
		else
		{
			SetMonsterType(TAG_CHAR::TAG_MUMMY_STANDING);
		}
		UpdateFindRange(40.f, 90.f);
		SetMoveSpeed(5);
		SetAttackRange(10.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 500 });
	}
	_bool CMummy::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f);
		SetAnimController(Create<CMummyAnimController>(GetCoreInstance(), ThisShared<CMummy>(),
			"..\\..\\Resource\\Anim\\Mummy\\", "Mummy_DEMO_1_FBX.bin", Matrix));
		SetCharStatus(CHARSTATUS{ 1, 0, 5 });
		return __super::Start(_ReceiveDatas);
	}

	void CMummy::Tick(const _double& _dTimeDelta)
	{
		__super::Tick(_dTimeDelta);
	}

	void CMummy::LateTick(const _double& _dTimeDelta)
	{
		__super::LateTick(_dTimeDelta);
	}

	void CMummy::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		__super::State(_spSession, _MonsterState);
	}

	void CMummy::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	bool CMummy::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return __super::IsHit(_pPawn, _dTimeDelta);
	}

	void CMummy::CallActiveEnable()
	{
	
	}

	void CMummy::CallActiveDisable()
	{
	}

	void CMummy::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CMummy::Free()
	{
	}

}