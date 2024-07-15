#include "CoreDefines.h"
#include "AMonster.h"
#include "ASession.h"
#include "ATransform.h"
#include "ACoreInstance.h"
#include "AJobTimer.h"
#include "AAnimController.h"
#include "ANavigation.h"

namespace Core
{
	AMonster::AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) :
		APawn(OBJCON_CONDATA, _ID, SESSIONTYPE::MONSTER), m_vNextPos{}, 
		m_MobState{ MOB_DISABLE_STATE }, m_RoomIndex{}, m_PathList{}, m_strAnimTriggerName{""}, m_strCurAnimName{""}, 
		m_wpMonsterJobTimer{_spMonsterJobTimer}, m_isFoundPlayerFistTime{false}, m_fActiveRange{0},
		m_fDeactiveRange{0}, m_iMonsterType{0}, m_fDistanceToPlayer{0},
		m_isCurrentAtkPlayer{ false }, m_isCurrentFindPlayer { false}, m_isCurrentJustMove{ false },
		m_fMoveSpeed { 0 }, m_CurrentTargetPlayerID{ -1 }, m_FindNextPosTimer{0.5, std::memory_order_seq_cst},
		m_TargetPosLock{},  m_iNextPathIndex{0}, m_vTargetPos{}, m_isPathFinding{false}
	{
		m_FindNextPosTimer.fTimer = 100.f;
		m_FindNextPosTimer.isPass = true;
	}
	_bool AMonster::Start(const VOIDDATAS& _ReceiveDatas)
	{
		return __super::Start(_ReceiveDatas);
	}

	void AMonster::FindPlayer(SHPTR<ASession> _spSession)
	{
		if (true == _spSession->IsPermanentDisable())
			return;

		SHPTR<ATransform> spMobTr = GetTransform();
		SHPTR<ATransform> spPlayerTr = _spSession->GetTransform();
		{
			Vector3 vMobPos = spMobTr->GetPos();
			Vector3 vPlayerPos = spPlayerTr->GetPos();
			Vector3 vDirection = vPlayerPos - vMobPos;

			m_fDistanceToPlayer = vDirection.LengthSquared();

			if (m_fDistanceToPlayer <= m_fAttackRange)
			{
				SetMonsterState(MONSTERSTATE::MOB_ATTACK_STATE);
				UpdateSelfStateToPlayerDistance(true, false, false);
			}
			else if (m_fDistanceToPlayer <= m_fActiveRange )
			{
				SetMonsterState(MONSTERSTATE::MOB_FIND_STATE);
				UpdateSelfStateToPlayerDistance(false, true, false);
			}
			else if (m_fDeactiveRange >= m_fDistanceToPlayer)
			{
				SetMonsterState(MONSTERSTATE::MOB_MOVE_STATE);
				UpdateSelfStateToPlayerDistance(false, false, true);
			}
		}
	}

	void AMonster::Tick(const _double& _dTimeDelta)
	{
		SHPTR<AAnimController> spAnimController = GetAnimController();
		spAnimController->Tick(_dTimeDelta);
		RestrictPositionToNavi();
	}


	void AMonster::InsertMobJobTimer(_int _PlayerID)
	{
		if (true == IsPermanentDisable())
			return;

		SetActive(true);

		SHPTR<AJobTimer> spJobTimer = m_wpMonsterJobTimer.lock();
		if (nullptr != spJobTimer)
		{
			spJobTimer->InsertTimerEvent(TIMEREVENT(GetSessionID(), _PlayerID, EVENT_TYPE::EV_MOB_ACTIVE));
		}
	}

	_bool AMonster::IsChangeTargetPlayer(SESSIONID _TargetPlayerID, _float _fDistance)
	{
		std::atomic_thread_fence(std::memory_order_seq_cst);
		SESSIONID CurrentTargetPlayerID = m_CurrentTargetPlayerID;
		_float DistanceToPlayer = m_fDistanceToPlayer;

		if (CurrentTargetPlayerID == _TargetPlayerID)
			return true;

		if (-1 == CurrentTargetPlayerID)
			return true;

		// 현재 감지하고 있는 플레이어가 인지 범위를 벗어나야 한다. 
		if (m_fDeactiveRange >= DistanceToPlayer)
		{
			if (DistanceToPlayer > _fDistance)
			{
				m_CurrentTargetPlayerID = _TargetPlayerID;
				return true;
			}
		}
		return false;
	}

	void AMonster::SetMonsterState(const MONSTERSTATE _MonsterState)
	{
		m_MobState = _MonsterState;

		if (nullptr != GetAnimController())
		{
			GetAnimController()->SetPawnState(m_MobState);
		}
	}

	void AMonster::SetFoundPlayerFirstTime(_bool _isFindFirstTime)
	{
		m_isFoundPlayerFistTime = _isFindFirstTime;
	}

	void AMonster::ComputeNextDir(const _double _dTimeDelta)
	{
		SHPTR<ANavigation> spNavigation = GetNavigation();
		SHPTR<ATransform> spSelfTr = GetTransform();
		Vector3 vPos = spSelfTr->GetPos();
		Vector3 vTargetPos;
		const static _float ROTATEVALUE = 5;
		{
			READ_SPINLOCK(m_TargetPosLock);
			vTargetPos = m_vTargetPos;
		}

		if (true == m_isCurrentFindPlayer)
		{
			// Find Path
			{
				if (m_FindNextPosTimer.IsOver(_dTimeDelta))
				{
					m_PathFindState = spNavigation->StartPathFinding(vPos, vTargetPos);
					m_FindNextPosTimer.ResetTimer();
					m_isPathFinding = true;
				}

				if (true == m_isPathFinding)
				{
					m_isRecvPathFindState = spNavigation->StepPathFinding(m_PathFindState);
					if (true == m_PathFindState.isPathFound && true == m_isRecvPathFindState)
					{
						m_PathList = std::move(spNavigation->ComputePath(m_PathFindState));
						m_iNextPathIndex = 0;
					}
					m_isPathFinding = false;
				}
			}

			_float fDistance = spSelfTr->ComputeDistanceSq(m_vNextPos);
			if (true == m_PathList.empty() || m_PathList.size() <= m_iNextPathIndex)
			{
				Vector3 vDirection = vTargetPos - spSelfTr->GetPos();
				vDirection.Normalize();
				GetTransform()->SetDirectionFixedUp(vDirection, (_float)_dTimeDelta, ROTATEVALUE);
			}
			else
			{
				if (m_PathList.size() > m_iNextPathIndex)
				{
					m_vNextPos = m_PathList[m_iNextPathIndex];
				}

				if (spSelfTr->ComputeDistanceSq(m_vNextPos) <= 10)
				{
					m_iNextPathIndex++;
					return;
				}
				Vector3 vDirection = m_vNextPos - spSelfTr->GetPos();
				vDirection.Normalize();
				GetTransform()->SetDirectionFixedUp(vDirection, (_float)_dTimeDelta, ROTATEVALUE);
			}
		}
		else if (true == m_isCurrentAtkPlayer)
		{
			Vector3 vDirection = vTargetPos - spSelfTr->GetPos();
			vDirection.Normalize();
			GetTransform()->SetDirectionFixedUp(vDirection, (_float)_dTimeDelta, ROTATEVALUE);
		}
	}

	void AMonster::UpdateTargetPos(SHPTR<ATransform> _ArriveTr)
	{
		{
			WRITE_SPINLOCK(m_TargetPosLock);
			m_vTargetPos = _ArriveTr->GetPos();
		}
	}

	void AMonster::UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange)
	{
		m_fActiveRange = _fActiveRange * _fActiveRange;
		m_fDeactiveRange = _fDeactiveRange * _fDeactiveRange;
	}

	void AMonster::UpdateSelfStateToPlayerDistance(_bool _isCurAtkPlayer, _bool _isCurFindPlayer, _bool _isCurJustMove)
	{
		m_isCurrentAtkPlayer = _isCurAtkPlayer;
		m_isCurrentFindPlayer = _isCurFindPlayer;
		m_isCurrentJustMove = _isCurJustMove;
	}

	void AMonster::Free()
	{
	}

}