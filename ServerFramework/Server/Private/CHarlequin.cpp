#include "ServerDefines.h"
#include "CHarlequin.h"
#include "ATransform.h"
#include "CHarlequinAnimController.h"

namespace Server {

	CHarlequin::CHarlequin(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) :
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_HARLEQUINN);
		UpdateFindRange(80, 150);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1500 });
	}

	_bool CHarlequin::Start(const VOIDDATAS& _ReceiveDatas)
	{
		SetAnimController(Create<CHarlequinAnimController>(GetCoreInstance(), ThisShared<CHarlequin>(),
			"..\\..\\Resource\\Anim\\Harlequin\\", "Harlequin1_FBX.bin"));

		return __super::Start(_ReceiveDatas);
	}

	void CHarlequin::Tick(const _double& _dTimeDelta)
	{
	}

	void CHarlequin::LateTick(const _double& _dTimeDelta)
	{
	}

	void CHarlequin::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	//	__super::State(_spSession, _MonsterState);
	}

	void CHarlequin::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	bool CHarlequin::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CHarlequin::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CHarlequin::Free()
	{
	}

}