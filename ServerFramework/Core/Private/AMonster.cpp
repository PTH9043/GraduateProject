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
		APawn(OBJCON_CONDATA, _ID, SESSIONTYPE::MONSTER),
		m_MobState{ MOB_DISABLE_STATE }, m_RoomIndex{}, m_strAnimTriggerName{""}, m_strCurAnimName{""}, 
		m_wpMonsterJobTimer{_spMonsterJobTimer}, m_isFoundPlayerFistTime{false}, m_fActiveRange{0},
		m_fDeactiveRange{0}, m_iMonsterType{0}, m_fDistanceToPlayer{-1},
		m_isCurrentAtkPlayer{ false }, m_isCurrentFindPlayer { false}, m_isCurrentNotFound{ false },
		m_fMoveSpeed { 0 }, m_OwnerMonsterSessionID{-1}, m_spTargetSession{nullptr},
		m_vTargetPos{}
	{
		SetActive(true);
	}
	_bool AMonster::Start(const VOIDDATAS& _ReceiveDatas)
	{
		return __super::Start(_ReceiveDatas);
	}

	void AMonster::FindPlayer(SHPTR<ASession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		SHPTR<ATransform> spMobTr = GetTransform();
		SHPTR<ATransform> spPlayerTr = _spSession->GetTransform();
		{
			Vector3 vMobPos = spMobTr->GetPos();
			Vector3 vPlayerPos = spPlayerTr->GetPos();
			Vector3 vDirection = vPlayerPos - vMobPos;

			float fDistanceToPlayer = vDirection.Length();
			fDistanceToPlayer = vDirection.Length();

			if (fDistanceToPlayer <= m_fActiveRange)
			{
				if(true == IsCurrentNotFound())
					m_spTargetSession = _spSession;

				if (fDistanceToPlayer <= m_fAttackRange)
				{
					UpdateSelfStateToPlayerDistance(true, true, false);
				}
				else
				{
					UpdateSelfStateToPlayerDistance(false, true, false);
				}
			}
			else if (m_fDeactiveRange >= fDistanceToPlayer)
			{
				UpdateSelfStateToPlayerDistance(false, false, true);
				m_spTargetSession = nullptr;
			}
		}
	}

	void AMonster::Tick(const _double& _dTimeDelta)
	{
	}

	void AMonster::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	}

	void AMonster::InsertMobJobTimer(SESSIONID _PlayerID)
	{
		if (true == IsPermanentDisable())
			return;

		SHPTR<AJobTimer> spJobTimer = m_wpMonsterJobTimer.lock();
		if (nullptr != spJobTimer)
		{
			SESSIONID SessionID = GetSessionID();
			spJobTimer->InsertTimerEvent(TIMEREVENT(SessionID, _PlayerID, EVENT_TYPE::EV_MOB_ACTIVE));
		}
	}

	_bool AMonster::IsCanMove(const Vector3 _vPos)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<ANavigation> spNaviagation = GetNavigation();
		SHPTR<ACell> spCell = nullptr;
		if (true == spNaviagation->IsMove(_vPos, spCell))
		{
			spTransform->SetPos(_vPos);
			return true;
		}
		return false;
	}

	void AMonster::ComputeNextDir(const _double _dTimeDelta)
	{
		SHPTR<ANavigation> spNavigation = GetNavigation();
		SHPTR<ATransform> spSelfTr = GetTransform();
	}

	void AMonster::UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange)
	{
		m_fActiveRange = _fActiveRange;
		m_fDeactiveRange = _fDeactiveRange;
	}

	void AMonster::UpdateSelfStateToPlayerDistance(_bool _isCurAtkPlayer, _bool _isCurFindPlayer, _bool _isCurNotFound)
	{
		m_isCurrentAtkPlayer = _isCurAtkPlayer;
		m_isCurrentFindPlayer = _isCurFindPlayer;
		m_isCurrentNotFound = _isCurNotFound;
	}

	void AMonster::ResetTargetPlayer()
	{
		RETURN_CHECK(nullptr == m_spTargetSession, ;);
		if (true == m_spTargetSession->IsPermanentDisable())
		{
			UpdateSelfStateToPlayerDistance(false, false, true);
			m_spTargetSession.reset();
		}
	}

	void AMonster::Free()
	{
	}

}