#include "ServerDefines.h"
#include "CMummy.h"
#include "ATransform.h"
#include "CMummyAnimController.h"
#include "CMobLayoutLoader.h"
#include "ANavigation.h"
#include "ACoreInstance.h"
#include "ACell.h"
#include "APathFinder.h"
#include "AAnimator.h"
#include "AAnimController.h"
#include "AAnimation.h"
#include "ASession.h"

namespace Server
{
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer)
		: AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer), 
		m_eMumyType{ _MummyType }, m_isAttackMode{ false }, m_isLastAttackWasFirst{ false },
		m_IdleTimer{2}, m_IdleRandomValueChooseTimer{2}, m_AttackTimer{3},
		m_iRandomValue{0}
	{
		if (MUMMY_LAYING == m_eMumyType)
		{
			SetMonsterType(TAG_CHAR::TAG_MUMMY_LAYING);
		}
		else
		{
			SetMonsterType(TAG_CHAR::TAG_MUMMY_STANDING);
		}
		UpdateFindRange(30.f, 80.f);
		SetMoveSpeed(5);
		SetAttackRange(15.f);
	}
	_bool CMummy::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CMummyAnimController>(GetCoreInstance(), ThisShared<CMummy>(),
			"..\\..\\Resource\\Anim\\Mummy\\", "Mummy_DEMO_1_FBX.bin", Matrix));
#ifndef CREATED_SERVERMOBDATA
		MOBDATA* pMobData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		// Setting Animation 
		GetAnimController()->SetAnimation(pMobData->strAnimName);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
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

	void CMummy::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AAnimController> spAnimController = GetAnimController();
		SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
		SHPTR<AAnimation> spCurAnimation = spAnimator->GetCurAnimation();
		SHPTR<ATransform> spTransform = GetTransform();
		_string strCurAnimName = spCurAnimation->GetAnimName();
		_bool Hit = false;

		spAnimController->ClearState();
		if (true == IsCurrentFindPlayer())
		{
			// wakeUp
			if (false == IsFoundPlayerFirstTime())
			{
				// Tick event
				spAnimController->UpdateState("WAKEUP", MUMMYANIMSTATE::MUMMY_ANIM_AWAKE);
				SetMonsterState(MOB_FIRSTFIND_STATE);
				SetFoundPlayerFirstTime(true);
			}
		}
		else
		{
			if (strCurAnimName == "idle")
			{
				if (m_IdleTimer.IsOver(_dTimeDelta))
				{
					m_IdleTimer.ResetTimer();
				}
			}
			else
			{
				m_IdleTimer.ResetTimer();
			}

			if (0 == m_IdleTimer.fTimer)
			{
				if (m_IdleRandomValueChooseTimer.IsOver(_dTimeDelta))
				{
					m_iRandomValue = std::rand() % 4;
				}

				if (0 == m_iRandomValue)
				{
					spAnimController->UpdateState("IDLE", ANIM_IDLE);
				}
				else
				{
					spAnimController->UpdateState("WALKF", ANIM_MOVE);
				}
			}
		}

		if (true == Hit)
		{
			spAnimController->UpdateState("HIT", ANIM_HIT);
			spAnimator->SetAnimation("gotHit");
		}
		else
		{
			if (true == m_isAttackMode)
			{
				if (m_AttackTimer.IsOver(_dTimeDelta))
				{
					m_isLastAttackWasFirst = !m_isLastAttackWasFirst;
					m_AttackTimer.ResetTimer();
				}

				if (false == IsCurrentAtkPlayer())
				{
					spAnimController->UpdateState("WALKF", ANIM_MOVE);
				}
				else
				{
					spAnimController->UpdateState(m_isLastAttackWasFirst ? "ATTACK02" : "ATTACK01", ANIM_ATTACK);
				}
			}
		}
	
		if (false == m_isAttackMode)
		{
			if ("openLaying" == strCurAnimName || "openStanding" == strCurAnimName)
			{
				spAnimController->UpdateState("TAUNT", MUMMYANIMSTATE::MUMMY_ANIM_TAUNT);
				SetMonsterState(MOB_FIND_STATE);
				m_isAttackMode = true;
			}
		}

		ComputeNextDir(_dTimeDelta);
		__super::Tick(_dTimeDelta);
	}

	void CMummy::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);

		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AAnimController> spAnimController = GetAnimController();
		SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
		SHPTR<AAnimation> spCurAnimation = spAnimator->GetCurAnimation();
		SHPTR<ATransform> spTransform = GetTransform();
		_string strCurAnimName = spCurAnimation->GetAnimName();

		if (true == IsCurrentFindPlayer())
		{
			UpdateTargetPos(_spSession->GetTransform());
		}

		VECTOR3 vRotate;
		VECTOR3 vPos;
		// Pos, Rotate를 만든다. 
		{
			Vector3 vRecvRotate = spTransform->GetRotationValue();
			Vector3 vRecvPos = spTransform->GetPos();

			PROTOFUNC::MakeVector3(&vRotate, vRecvRotate.x, vRecvRotate.y, vRecvRotate.z);
			PROTOFUNC::MakeVector3(&vPos, vRecvPos.x, vRecvPos.y, vRecvPos.z);
		}
		// Send Packet
		SC_MONSTERSTATEHAVEPOS scMonsterStateHavePos;
		{
			_int AnimIndex = spAnimController->GetCurAnimIndex();
			_double dTimeAcc = spCurAnimation->GetTimeAcc();
			PROTOFUNC::MakeScMonsterStateHavePos(&scMonsterStateHavePos, GetSessionID(), vPos, vRotate,
				dTimeAcc, AnimIndex, GetMonsterState());
			CombineProto<SC_MONSTERSTATEHAVEPOS>(GetCopyBuffer(), GetPacketHead(), scMonsterStateHavePos, TAG_SC_MONSTERSTATEHAVEMOVE);
			spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
		}
	}

	bool CMummy::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CMummy::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{

	}

	void CMummy::Free()
	{
	}

}