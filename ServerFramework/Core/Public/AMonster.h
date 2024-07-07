#pragma once
#include "APawn.h"

BEGIN(Core)
class ASession;
class ACell;
class AJobTimer;

using ANIMFUNC = std::function<void*>(const _double&);

class CORE_DLL AMonster abstract : public APawn {
	using CELLCONTAINER = CONLIST<SHPTR<ACell>>;
public:
	AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(AMonster)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	void FindPlayer(SHPTR<ASession> _spSession, const _float _fFindDistanceValue);
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) PURE;
	void InsertMobJobTimer(_int _PlayerID);
	void InsertPathList(LIST<SHPTR<ACell>>& _PathList);
public: /* get set */
	const MONSTERSTATE GetMonsterState() const { return m_MobState; }
	const Vector3 GetNextPos() const { return m_vNextPos; }
	const _int GetMonsterType() const { return m_iMonsterType; }

	void SetNextPos(const Vector3 _vNextPos) { this->m_vNextPos = _vNextPos; }
	void SetMonsterState(const MONSTERSTATE _MonsterState) { this->m_MobState = _MonsterState; }
protected: /* get set*/
	SHPTR<AJobTimer> GetMonsterJobTimer() { return m_wpMonsterJobTimer.lock(); }

	void SetMonsterJobTimer(SHPTR<AJobTimer> _spJobTimer) { m_wpMonsterJobTimer = _spJobTimer; }
	void SetMonsterType(_int _iMonsterType) { this->m_iMonsterType = _iMonsterType; }
private:
	virtual void Free() override;
private:
	// 가야할 다음 장소
	Vector3											m_vNextPos;
	ATOMIC<MONSTERSTATE>		m_MobState;
	_int													m_RoomIndex;
	_int													m_iMonsterType;

	CELLCONTAINER							m_PathList;

	_string											m_strAnimTriggerName;
	_string											m_strCurAnimName;

	WKPTR<AJobTimer>					m_wpMonsterJobTimer;
};


END