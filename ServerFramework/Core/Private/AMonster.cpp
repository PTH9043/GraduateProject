#include "CoreDefines.h"
#include "AMonster.h"
#include "ASession.h"
#include "ATransform.h"
#include "ACoreInstance.h"
#include "AJobTimer.h"

namespace Core
{
	AMonster::AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID) :
		APawn(OBJCON_CONDATA, _ID, SESSIONTYPE::MONSTER), m_vNextPos{},
		m_MobState{ MOB_END }, m_RoomIndex{}, m_PathList{}, m_strAnimTriggerName{""}, m_strCurAnimName{""}
	{
	
	}
	void AMonster::FindPlayer(SHPTR<ASession> _spSession, const _float _fFindDistanceValue)
	{
		SHPTR<ATransform> spMobTr = GetTransform();
		SHPTR<ATransform> spPlayerTr = _spSession->GetTransform();
		{
			Vector3 vMobPos = spMobTr->GetPos();
			Vector3 vPlayerPos = spPlayerTr->GetPos();
			Vector3 vDirection = vPlayerPos - vMobPos;

			float fDistance = vDirection.LengthSquared();
			if (fDistance <= _fFindDistanceValue)
			{
				SetMonsterState(MONSTERSTATE::MOB_FIND);
			}
		}
	}
	void AMonster::InsertMobJobTimer(_int _PlayerID)
	{
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

	void AMonster::Free()
	{
	}

}