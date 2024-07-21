#include "ServerDefines.h"
#include "CAnubis.h"
#include "ATransform.h"

namespace Server
{

	CAnubis::CAnubis(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_ANUBIS);
		UpdateFindRange(40.f, 60.f);
		SetMoveSpeed(10);
		SetAttackRange(20.f);
		SetActive(false);
	}
	_bool CAnubis::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
#ifndef CREATED_SERVERMOBDATA
		MOBDATA* pMobData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		// Setting Animation 
		// GetTransform()->SetPos(pMobData->mWorldMatrix.Get_Pos());
		GetTransform()->SetDirection(pMobData->mWorldMatrix.Get_Look());
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
#else
		MOBSERVERDATA* pMobData = static_cast<MOBSERVERDATA*>(_ReceiveDatas[0]);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		SetSessionID(pMobData->iMobID);
#endif
		return true;
	}

	void CAnubis::Tick(const _double& _dTimeDelta)
	{
	}

	void CAnubis::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	}

	void CAnubis::ProcessPacket(_int _type, void* _pData)
	{
	}

	bool CAnubis::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CAnubis::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CAnubis::ChangeCurrentFindPlayer(SESSIONID _CurPlayerSessionID, SESSIONID _ChangePlayerSessionID)
	{
	}

	void CAnubis::Free()
	{
	}

}