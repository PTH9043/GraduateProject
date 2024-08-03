#include "ServerDefines.h"
#include "CMinotaur.h"
#include "ATransform.h"
#include "CMinotaurAnimController.h"

namespace Server {

	CMinotaur::CMinotaur(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_MINOTAUR);
		UpdateFindRange(100, 120);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 2500 });
	}

	_bool CMinotaur::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) ;
		SetAnimController(Create<CMinotaurAnimController>(GetCoreInstance(), ThisShared<CMinotaur>(),
			"..\\..\\Resource\\Anim\\MinoTaur\\", "minotaur_FBX.bin", Matrix));

		return __super::Start(_ReceiveDatas);
	}

	void CMinotaur::Tick(const _double& _dTimeDelta)
	{
	}

	void CMinotaur::LateTick(const _double& _dTimeDelta)
	{
	}

	void CMinotaur::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
//		__super::State(_spSession, _MonsterState);
	}

	void CMinotaur::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	bool CMinotaur::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CMinotaur::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CMinotaur::Free()
	{
	}

}