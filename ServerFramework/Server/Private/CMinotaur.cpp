#include "ServerDefines.h"
#include "CMinotaur.h"
#include "ATransform.h"
#include "CMinotaurAnimController.h"

namespace Server {

	CMinotaur::CMinotaur(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_MINOTAUR);
		UpdateFindRange(40.f, 60.f);
		SetMoveSpeed(10);
		SetAttackRange(20.f);
		SetActive(false);
	}

	_bool CMinotaur::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
		SetAnimController(Create<CMinotaurAnimController>(GetCoreInstance(), ThisShared<CMinotaur>(),
			"..\\..\\Resource\\Anim\\MinoTaur\\", "minotaur_FBX.bin"));
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
		GetAnimController()->SetAnimation(pMobData->iStartAnimIndex);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		SetSessionID(pMobData->iMobID);
#endif
		return true;
	}

	void CMinotaur::Tick(const _double& _dTimeDelta)
	{
	}

	void CMinotaur::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	}

	void CMinotaur::ProcessPacket(_int _type, void* _pData)
	{
	}

	bool CMinotaur::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CMinotaur::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CMinotaur::ChangeCurrentFindPlayer(SESSIONID _CurPlayerSessionID, SESSIONID _ChangePlayerSessionID)
	{
	}

	void CMinotaur::Free()
	{
	}

}