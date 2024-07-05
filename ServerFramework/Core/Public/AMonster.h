#pragma once
#include "APawn.h"

BEGIN(Core)
class ASession;
class ACell;
class AJobTimer;

class CORE_DLL AMonster abstract : public APawn {
	using CELLCONTAINER = CONLIST<SHPTR<ACell>>;
public:
	AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(AMonster)
public:
	virtual _bool Start() PURE;
	void FindPlayer(SHPTR<ASession> _spSession, const _float _fFindDistanceValue);
	virtual void State() PURE;
	void InsertMobJobTimer(_int _PlayerID);
	void InsertPathList(LIST<SHPTR<ACell>>& _PathList);
public: /* get set */
	const MONSTERSTATE GetMonsterState() const { return m_MobState; }
	const Vector3 GetNextPos() const { return m_vNextPos; }

	void SetNextPos(const Vector3 _vNextPos) { this->m_vNextPos = _vNextPos; }
	void SetMonsterState(const MONSTERSTATE _MonsterState) { this->m_MobState = _MonsterState; }
protected: /* get set*/
	SHPTR<AJobTimer> GetMonsterJobTimer() { return m_wpMonsterJobTimer.lock(); }

	void SetMonsterJobTimer(SHPTR<AJobTimer> _spJobTimer) { m_wpMonsterJobTimer = _spJobTimer; }
private:
	virtual void Free() override;
private:
	// 가야할 다음 장소
	Vector3											m_vNextPos;
	ATOMIC<MONSTERSTATE>		m_MobState;
	_int													m_RoomIndex;

	CELLCONTAINER							m_PathList;

	_string											m_strAnimTriggerName;
	_string											m_strCurAnimName;

	WKPTR<AJobTimer>					m_wpMonsterJobTimer;
};


END