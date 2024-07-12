#include "CoreDefines.h"
#include "AMonster.h"
#include "ASession.h"
#include "ATransform.h"
#include "ACoreInstance.h"
#include "AJobTimer.h"
#include "AAnimController.h"

namespace Core
{
	AMonster::AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) :
		APawn(OBJCON_CONDATA, _ID, SESSIONTYPE::MONSTER), m_vNextPos{},
		m_MobState{ MOB_DISABLE_STATE }, m_RoomIndex{}, m_PathList{}, m_strAnimTriggerName{""}, m_strCurAnimName{""}, 
		m_wpMonsterJobTimer{_spMonsterJobTimer}, m_isFoundPlayerFistTime{false}, m_fActiveRange{0},
		m_fDeactiveRange{0}, m_iMonsterType{0}, m_fDistanceToPlayer{0},
		m_isCurrentFindPlayer{false}, m_fMoveSpeed{0}, m_CurrentTargetPlayerID{-1}
	{
	
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
			SetCurrentFindPlayer(false);

			if (m_fDistanceToPlayer <= m_fAttackRange)
			{
				SetMonsterState(MONSTERSTATE::MOB_ATTACK_STATE);
			}
			else if (m_fDistanceToPlayer <= m_fActiveRange )
			{
				SetMonsterState(MONSTERSTATE::MOB_FIND_STATE);
				SetCurrentFindPlayer(true);
			}
			else if (m_fDeactiveRange >= m_fDistanceToPlayer)
			{
				SetMonsterState(MONSTERSTATE::MOB_MOVE_STATE);
			}
		}
	}

	void AMonster::Tick(const _double& _dTimeDelta)
	{
		SHPTR<AAnimController> spAnimController = GetAnimController();
		spAnimController->Tick(_dTimeDelta);
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

		if (DistanceToPlayer > _fDistance)
		{
			m_CurrentTargetPlayerID = _TargetPlayerID;
			return true;
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

	void AMonster::InsertPathList(LIST<Vector3>&& _PathList)
	{
		std::atomic_thread_fence(std::memory_order_seq_cst);
		m_PathList.clear();

		for (LIST<Vector3>::reverse_iterator it = _PathList.rbegin(); it != _PathList.rend(); ++it)
		{
			m_PathList.Push(*it);
		}
	}

	void AMonster::UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange)
	{
		m_fActiveRange = _fActiveRange * _fActiveRange;
		m_fDeactiveRange = _fDeactiveRange * _fDeactiveRange;
	}

	void AMonster::MoveToNextPos(const _double& _dTimeDelta)
	{
		if (false == m_PathList.IsEmpty())
		{
			m_vNextPos = m_PathList.Pop();
			GetTransform()->TranslatePos(m_vNextPos, _dTimeDelta, m_fMoveSpeed);
		}
	}

	void AMonster::SetTargetPlayerID(const SESSIONID _SessionID)
	{
		m_CurrentTargetPlayerID = _SessionID;
	}

	void AMonster::SetCurrentFindPlayer(const _bool _isFindPlayer)
	{
		m_isCurrentFindPlayer = _isFindPlayer;
	}

	void AMonster::Free()
	{
	}

}