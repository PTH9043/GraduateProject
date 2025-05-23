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
		__super::Start(_ReceiveDatas);
		SetAnimController(Create<CAnubisAnimController>(GetCoreInstance(), ThisShared<CAnubis>(),
			"..\\..\\Resource\\Anim\\Anubis\\", "Anubis_FBX.bin"));
#ifndef CREATED_SERVERMOBDATA
		MOBDATA* pMobData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		// Setting Animation 
		GetAnimController()->SetAnimation(pMobData->strAnimName);
		GetTransform()->SetPos(pMobData->mWorldMatrix.Get_Pos());
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
		__super::State(_spSession, _MonsterState);
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

	void CAnubis::ChangeCurrentFindPlayer(SESSIONID _CurPlayerSessionID, SESSIONID _ChangePlayerSessionID)
	{
	}

	void CAnubis::Free()
	{
	}

}