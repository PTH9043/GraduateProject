#include "ServerDefines.h"
#include "CMimic.h"
#include "ATransform.h"
#include "CMimicAnimController.h"

namespace Server {

	CMimic::CMimic(OBJCON_CONSTRUCTOR, SESSIONID _ID,
		SHPTR<AJobTimer> _spMonsterJobTimer) : 
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_MIMIC);
		UpdateFindRange(10, 120);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 200 });
	}

	_bool CMimic::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) ;
		SetAnimController(Create<CMimicAnimController>(GetCoreInstance(), ThisShared<CMimic>(),
			"..\\..\\Resource\\Anim\\Mimic\\", "Mimic_FBX.bin", Matrix));

		return __super::Start(_ReceiveDatas);
	}

	void CMimic::Tick(const _double& _dTimeDelta)
	{
	}

	void CMimic::LateTick(const _double& _dTimeDelta)
	{
	}

	void CMimic::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	//	__super::State(_spSession, _MonsterState);
	}

	void CMimic::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	bool CMimic::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CMimic::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CMimic::Free()
	{
	}
}
