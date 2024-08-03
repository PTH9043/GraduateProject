#include "ServerDefines.h"
#include "CServerMonster.h"
#include "ATransform.h"
#include "AAnimController.h"
#include "AJobTimer.h"
#include "AAnimController.h"
#include "ANavigation.h"
#include "ACell.h"
#include "ASession.h"
#include "AAnimController.h"
#include "AAnimator.h"
#include "ACoreInstance.h"
#include "AAnimation.h"

namespace Server {

	CServerMonster::CServerMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer), 
		 m_iNextPathIndex{ 0 }, m_isPathFinding{ false }, m_TimeAccumulator{ 1, 5 }, 
		m_dDeadAnimSpeed{20}, m_dElapsedTime{0}, m_dDeadTimeArcOpenEnd{500}
	{
	}

	_bool CServerMonster::Start(const VOIDDATAS& _ReceiveDatas)
	{
		SHPTR<ANavigation> spNavigation = GetNavigation();
#ifndef CREATED_SERVERMOBDATA
		MOBDATA* pMobData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		// Setting Animation 
		GetAnimController()->SetAnimation(pMobData->strAnimName);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		GetTransform()->SetPos(BringCellIndextoPosition());
#else
		MOBSERVERDATA* pMobData = static_cast<MOBSERVERDATA*>(_ReceiveDatas[0]);
		GetAnimController()->SetAnimation(pMobData->iStartAnimIndex);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		GetTransform()->SetPos(BringCellIndextoPosition());
		SetSessionID(pMobData->iMobID);
#endif
		SetPrevPosition(GetTransform()->GetPos());
		return __super::Start(_ReceiveDatas);
	}

	void CServerMonster::Tick(const _double& _dTimeDelta)
	{
		_llong CurrentTargetPlayerID = GetCurrentTargetPlayerID();
		RETURN_CHECK(0 >= CurrentTargetPlayerID, ;);
		{
			SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
			SHPTR<ASession> spPlayerSession = spCoreInstance->FindSession(CurrentTargetPlayerID);
			TickUpdateBehavior(_dTimeDelta, spPlayerSession);
		}
	}

	void CServerMonster::LateTick(const _double& _dTimeDelta)
	{
		RestrictPositionToNavi();
	}

	void CServerMonster::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

		_bool isCurrentFindPlayer = IsCurrentFindPlayer();
		_bool isDamaged = IsDamaged();

		if (true == IsDeadStateEnable())
		{
			// 영구적 비활성화
			ActivePermanentDisable();
		}
		else
		{
			SHPTR<ATransform> spTransform = GetTransform();
			SHPTR<AAnimController> spAnimController = GetAnimController();
			SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
			SHPTR<AAnimation> spCurAnimation = spAnimator->GetCurAnimation();

			Vector3 vPos = spTransform->GetPos();
			Vector3 vRotate = spTransform->GetRotationValue();

			VECTOR3 vSendPos, vSendRotate;
			{
				PROTOFUNC::MakeVector3(&vSendPos, vPos.x, vPos.y, vPos.z);
				PROTOFUNC::MakeVector3(&vSendRotate, vRotate.x, vRotate.y, vRotate.z);
			}

			_int AnimState = spAnimController->GetAnimState();
			_double dTimeAcc = spCurAnimation->GetTimeAcc();
			_int AnimIndex = spAnimator->GetCurAnimIndex();

			std::cout << "MUMMY Anim : " << AnimIndex << "\n";

			MOBSTATE monsterState;
			PROTOFUNC::MakeMobState(&monsterState, GetSessionID(), vSendPos, vSendRotate,
				AnimState, AnimIndex, false, isCurrentFindPlayer, isDamaged, dTimeAcc);
			CombineProto<MOBSTATE>(GetCopyBuffer(), GetPacketHead(), monsterState, TAG_SC_MONSTERSTATE);
			_spSession->SendData(GetCopyBufferPointer(), GetPacketHead());
		}
	}

	void CServerMonster::ProcessPacket(_int _type, void* _pData)
	{
		switch (_type)
		{
		case TAG_CS_MONSTERSTATE:
		{
			//MOBSTATE* MonsterStateData = static_cast<MOBSTATE*>(_pData);
			//SHPTR<ATransform> spTransform = GetTransform();
			//SHPTR<AAnimController> spAnimController = GetAnimController();
			//{
			//	spTransform->SetPos({ MonsterStateData->posx(), MonsterStateData->posy(), MonsterStateData->posz() });
			//	spTransform->RotateFix({ MonsterStateData->rotatex(), MonsterStateData->rotatey(), MonsterStateData->rotatez() });
			//	spAnimController->SetAnimation(MonsterStateData->animationindex());
			//}
		}
		break;
		}
	}

	bool CServerMonster::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CServerMonster::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CServerMonster::TickUpdateBehavior(const _double _dTimeDelta, SHPTR<ASession> _spSession)
	{
		SHPTR<AAnimController> spAnimController = GetAnimController();
		SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<ANavigation> spNaviagation = GetNavigation();
		SHPTR<ACell> spCurrentMobCell = GetCurCell();
		SHPTR<ACell> spTargetCell = nullptr;
		Vector3 vCurrentMobPos = spTransform->GetPos();
		Vector3 vTargetPos;

		SetPrevPosition(vCurrentMobPos);

		_bool isFoudnTargetState = IsCurrentFindPlayer();
		_bool isDeadState = IsDead();
		MONSTERSTATE MobState = GetMonsterState();

		spAnimController->Tick(_dTimeDelta);

		if (MOB_MOVE_STATE == MobState)
		{
			if (false == isDeadState)
			{
				if (true == isFoudnTargetState)
				{
					SHPTR<ATransform> spTargetTr = _spSession->GetTransform();
					spTargetCell = _spSession->GetCurCell();
					vTargetPos = spTargetCell->GetCenterPos();

					if (m_TimeAccumulator.IsOverLow())
					{
						FindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}
				}
				else
				{
					spTargetCell = spNaviagation->ChooseRandomNeighborCell(3);
					vTargetPos = spTargetCell->GetCenterPos();

					if (m_TimeAccumulator.IsOverHigh())
					{
						FindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}
				}
				MoveAlongPath(_dTimeDelta);
			}
		}
		else if (MOB_ATTACK_STATE == MobState)
		{
			Vector3 Direction = vTargetPos - vCurrentMobPos;
			Direction.Normalize();
			spTransform->SetDirectionFixedUp(Direction * -1, _dTimeDelta, GetMoveSpeed());
		}
		 
		if (MOB_DEATH_STATE == MobState)
		{
			SetElapsedTime(GetElapsedTime() + (_dTimeDelta * m_dDeadAnimSpeed));
			if (GetElapsedTime() < m_dDeadTimeArcOpenEnd)
			{
				spAnimator->TickAnimToTimeAccChangeTransform(spTransform, _dTimeDelta, GetElapsedTime());
			}
		}
		else if (MOB_IDLE_STATE == MobState)
		{
			spAnimator->TickAnimation(_dTimeDelta);
		}
		else
		{
			spAnimator->TickAnimChangeTransform(spTransform, _dTimeDelta);
			SetElapsedTime(0);
		}

		m_TimeAccumulator.PlusTime(_dTimeDelta);
	}

	void CServerMonster::MoveAlongPath(const _double& _dTimeDelta)
	{
		size_t pathSize = m_AstarPath.size();
		RETURN_CHECK(0 == pathSize, ;);

		_int CurrentPathIndex = m_iNextPathIndex;
		_float MoveSpeed = GetMoveSpeed();

		SHPTR<ATransform> spTransform = GetTransform();
		Vector3 vCurrentPos = spTransform->GetPos();
		Vector3 vTargetPos;
		Vector3 Direction;
		if (CurrentPathIndex <= pathSize)
		{
			vTargetPos = m_AstarPath[CurrentPathIndex];

			Direction = vTargetPos - vCurrentPos;
			_float fDistance = Direction.Length();
			Direction.Normalize();

			if (fDistance <= 1.0f)
			{
				m_iNextPathIndex.fetch_add(1);
			}
		}
		else
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			vTargetPos = GetTargetPos();
			Direction = vTargetPos - vCurrentPos;
			Direction.Normalize();
		}

		spTransform->SetDirectionFixedUp(Direction * -1, _dTimeDelta, MoveSpeed);
	}

	void CServerMonster::FindPath(SHPTR<ANavigation> _spNavigation, SHPTR<ACell> _spSelfCell, SHPTR<ACell> _spTargetCell, 
		const Vector3 _vSelfPos, const Vector3 _vTargetPos)
	{
		PathFindingState PathFind = _spNavigation->StartPathFinding(_vSelfPos, _vTargetPos,
			_spSelfCell, _spTargetCell);
		if (true == _spNavigation->StepPathFinding(PathFind))
		{
			if (true == PathFind.isPathFound)
			{
				m_AstarPath = _spNavigation->OptimizePath(PathFind.path, _vSelfPos, _vTargetPos);
				m_iNextPathIndex = 0;
			}
		}
		m_TimeAccumulator.ResetTimer();
	}


	void CServerMonster::Free()
	{
	}

}