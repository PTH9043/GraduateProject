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
		m_isCurrentAtkPlayer{ false }, m_isCurrentFindPlayer { false}, m_isCurrentJustMove{ false },
		m_fMoveSpeed { 0 }, m_CurrentTargetPlayerID{ -1 },  m_vTargetPos{},
		m_OwnerMonsterSessionID{-1}
	{
		SetActive(true);
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

			m_fDistanceToPlayer = vDirection.Length();

			if (m_fDistanceToPlayer <= m_fAttackRange)
			{
				UpdateSelfStateToPlayerDistance(true, false, false);
				ChangePlayerTargetID(_spSession->GetSessionID());
			}
			else if (m_fDistanceToPlayer <= m_fActiveRange)
			{
				UpdateSelfStateToPlayerDistance(false, true, false);
				ChangePlayerTargetID(_spSession->GetSessionID());
			}
			else if (m_fDeactiveRange >= m_fDistanceToPlayer)
			{
				UpdateSelfStateToPlayerDistance(false, false, true);
				m_CurrentTargetPlayerID.store(-1);
			}
		}
	}

	void AMonster::Tick(const _double& _dTimeDelta)
	{
		SHPTR<AAnimController> spAnimController = GetAnimController();
		spAnimController->Tick(_dTimeDelta);
		RestrictPositionToNavi();
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

	void AMonster::UpdateSelfStateToPlayerDistance(_bool _isCurAtkPlayer, _bool _isCurFindPlayer, _bool _isCurJustMove)
	{
		m_isCurrentAtkPlayer = _isCurAtkPlayer;
		m_isCurrentFindPlayer = _isCurFindPlayer;
		m_isCurrentJustMove = _isCurJustMove;
	}

	void AMonster::ChangePlayerTargetID(SESSIONID _SessionID)
	{
		if (-1 == m_CurrentTargetPlayerID)
		{
			m_CurrentTargetPlayerID = _SessionID;
		}
	}

	void AMonster::Free()
	{
	}

}