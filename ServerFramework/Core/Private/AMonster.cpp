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
		m_MobState{ MOB_END }, m_RoomIndex{}, m_PathList{}, m_strAnimTriggerName{""}, m_strCurAnimName{""}, 
		m_wpMonsterJobTimer{_spMonsterJobTimer}, m_isFoundPlayerFistTime{false}, m_fActiveRange{0},
		m_fDeactiveRange{0}, m_iMonsterType{0}, m_fDistanceToPlayer{0}
	{
	
	}
	_bool AMonster::Start(const VOIDDATAS& _ReceiveDatas)
	{
		return __super::Start(_ReceiveDatas);
	}

	_bool AMonster::IsFindPlayer(SHPTR<ASession> _spSession)
	{
		if (true == _spSession->IsPermanentDisable())
			return false;

		SHPTR<ATransform> spMobTr = GetTransform();
		SHPTR<ATransform> spPlayerTr = _spSession->GetTransform();
		{
			Vector3 vMobPos = spMobTr->GetPos();
			Vector3 vPlayerPos = spPlayerTr->GetPos();
			Vector3 vDirection = vPlayerPos - vMobPos;

			m_fDistanceToPlayer = vDirection.LengthSquared();
			if (m_fDistanceToPlayer <= m_fActiveRange )
			{
				SetMonsterState(MONSTERSTATE::MOB_FIND);
			}
			else if (m_fDeactiveRange >= m_fDistanceToPlayer)
			{
				SetMonsterState(MONSTERSTATE::MOB_MOVE);
			}
		}
		return false;
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

	void AMonster::InsertPathList(LIST<SHPTR<ACell>>& _PathList)
	{
		m_PathList.Clear();
		for (auto& iter : _PathList)
		{
			m_PathList.Insert(iter);
		}
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
		_bool isFound = IsFoundPlayerFirstTime();
		if (_isFindFirstTime == isFound)
			return;

		while (true)
		{
			isFound = m_isFoundPlayerFistTime.load();
			if (m_isFoundPlayerFistTime.compare_exchange_strong(isFound, _isFindFirstTime))
				break;
		}
	}

	void AMonster::UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange)
	{
		m_fActiveRange = _fActiveRange * _fActiveRange;
		m_fDeactiveRange = _fDeactiveRange * _fDeactiveRange;
	}

	void AMonster::Free()
	{
	}

}