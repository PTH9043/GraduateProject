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
		m_wpMonsterJobTimer{_spMonsterJobTimer}, m_fActiveRange{0}, m_fDeactiveRange{0}, m_iMonsterType{0}
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

			float fDistance = vDirection.LengthSquared();
			if (fDistance <= m_fActiveRange )
			{
				SetMonsterState(MONSTERSTATE::MOB_FIND);
			}
			else if (m_fDeactiveRange >= fDistance)
			{
				SetMonsterState(MONSTERSTATE::MOB_MOVE);
			}
		}
	}

	void AMonster::InsertMobJobTimer(_int _PlayerID)
	{
		SetActiveStrong(true);
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

	void AMonster::UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange)
	{
		m_fActiveRange = _fActiveRange * _fActiveRange;
		m_fDeactiveRange = _fDeactiveRange * _fDeactiveRange;
	}

	void AMonster::Free()
	{
	}

}