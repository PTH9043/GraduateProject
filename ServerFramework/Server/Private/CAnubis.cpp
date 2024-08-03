#include "ServerDefines.h"
#include "CAnubis.h"
#include "ATransform.h"
#include "CAnubisAnimController.h"

namespace Server
{

	CAnubis::CAnubis(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_ANUBIS);
		UpdateFindRange(70, 80);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1000 });
	}
	_bool CAnubis::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f);
		SetAnimController(Create<CAnubisAnimController>(GetCoreInstance(), ThisShared<CAnubis>(),
			"..\\..\\Resource\\Anim\\Anubis\\", "Anubis_FBX.bin", Matrix));
		return  __super::Start(_ReceiveDatas);
	}

	void CAnubis::Tick(const _double& _dTimeDelta)
	{
	}

	void CAnubis::LateTick(const _double& _dTimeDelta)
	{
	}

	void CAnubis::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	//	__super::State(_spSession, _MonsterState);
	}

	void CAnubis::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	bool CAnubis::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CAnubis::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CAnubis::Free()
	{
	}

}