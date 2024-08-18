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
	AMonster::PLAYERPOSITIONCONTAINER AMonster::s_PlayerPositionContainer{};
	AMonster::PLAYERFINDWEIGHTCONTAINER AMonster::s_PlayerFindWeightContainer{};

	AMonster::AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) :
		APawn(OBJCON_CONDATA, _ID, SESSIONTYPE::MONSTER),
		m_MobState{ MOB_DISABLE_STATE }, m_RoomIndex{}, m_strAnimTriggerName{""}, m_strCurAnimName{""}, 
		m_wpMonsterJobTimer{_spMonsterJobTimer}, m_isFoundPlayerFistTime{false}, m_fActiveRange{0},
		m_fDeactiveRange{0}, m_iMonsterType{0}, m_fDistanceToPlayer{-1},
		m_isCurrentAtkPlayer{ false }, m_isCurrentFindPlayer { false}, m_isCurrentNotFound{ false },
		m_fMoveSpeed { 0 }, m_OwnerMonsterSessionID{-1}, m_spTargetSession{nullptr},
		m_vTargetPos{}, m_isDamageEnable{false}
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
		SHPTR<AAnimController> spAnimController = GetAnimController();
		SHPTR<ATransform> spMobTr = GetTransform();
		SHPTR<ASession> spTargetSession = m_spTargetSession;
		SESSIONID SessionID = _spSession->GetSessionID();
		{
			_float ActiveRange = m_fActiveRange;
			_float DeactiveRange = m_fDeactiveRange;

			if(_spSession == spTargetSession || nullptr == spTargetSession)
			 {
				if (nullptr == spTargetSession)
					spTargetSession = _spSession;

				SHPTR<ATransform> spPlayerTr = spTargetSession->GetTransform();
				Vector3 vMobPos = spMobTr->GetPos();
				Vector3 vPlayerPos = spPlayerTr->GetPos();
				s_PlayerPositionContainer[SessionID] = vPlayerPos;
				float fDistanceToPlayer = Vector3::Distance(vMobPos, vPlayerPos);


				if (fDistanceToPlayer <= ActiveRange)
				{
					if (nullptr != spTargetSession)
					{
						if (spTargetSession->IsFallDownState())
							SetTargetSession(nullptr);
					}
					else
					{
						SetTargetSession(_spSession);
					}

					if (fDistanceToPlayer <= m_fAttackRange)
					{
						UpdateSelfStateToPlayerDistance(true, true, false);
					}
					else
					{
						UpdateSelfStateToPlayerDistance(false, true, false);
					}
					ChangeAtomicValue(m_fJudgeValueToPlayerDistance, fDistanceToPlayer);
				}
				else if (DeactiveRange >= fDistanceToPlayer)
				{
					UpdateSelfStateToPlayerDistance(false, false, true);
					SetTargetSession(nullptr);
				}
			}
			else
			{
				SHPTR<ATransform> spPlayerTr = _spSession->GetTransform();
				Vector3 vMobPos = spMobTr->GetPos();
				Vector3 vPlayerPos = spPlayerTr->GetPos();
				s_PlayerPositionContainer[SessionID] = vPlayerPos;
				float fDistanceToPlayer = Vector3::Distance(vMobPos, vPlayerPos);

				if (m_fJudgeValueToPlayerDistance >= fDistanceToPlayer)
				{
					if (fDistanceToPlayer <= ActiveRange)
					{
						if (fDistanceToPlayer <= m_fAttackRange)
						{
							UpdateSelfStateToPlayerDistance(true, true, false);
						}
						else
						{
							UpdateSelfStateToPlayerDistance(false, true, false);
						}
						SetTargetSession(m_spTargetSession);
						ChangeAtomicValue(m_fJudgeValueToPlayerDistance, fDistanceToPlayer);
					}
				}
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

	void AMonster::SetDirectionFixedUp(const _double& _dTimeDelta, const _float _fCurrentDot, 
		const _float _JudgaeDot, const Vector3& _vTargetPos)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		_float fRot = _fCurrentDot;
		if (fRot >= _JudgaeDot && fRot >= -_JudgaeDot)
			spTransform->LookAt(_vTargetPos);
		else
			spTransform->SetDirectionFixedUp((_vTargetPos - spTransform->GetPos()), static_cast<_float>(_dTimeDelta), ROT_SPEED);
	}

	void AMonster::UpdatePlayerToWeight(const SESSIONID _SessionID, const _float _fWeight)
	{
		s_PlayerFindWeightContainer[_SessionID] += _fWeight;
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
		ChangeAtomicValue(REF_OUT m_isCurrentAtkPlayer, _isCurAtkPlayer);
		ChangeAtomicValue(REF_OUT m_isCurrentFindPlayer, _isCurFindPlayer);
		ChangeAtomicValue(REF_OUT m_isCurrentNotFound, _isCurNotFound);
	}

	void AMonster::ResetTargetPlayer()
	{
		SHPTR<ASession> spSession = m_spTargetSession;
		RETURN_CHECK(nullptr == spSession, ;);
		if (true == spSession->IsPermanentDisable())
		{
			UpdateSelfStateToPlayerDistance(false, false, true);
			m_spTargetSession.reset();
		}
	}

	void AMonster::Free()
	{
	}

}