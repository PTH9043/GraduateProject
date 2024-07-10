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
	AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(AMonster)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	void FindPlayer(SHPTR<ASession> _spSession);
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) PURE;
	void InsertMobJobTimer(_int _PlayerID);
	void InsertPathList(LIST<SHPTR<ACell>>& _PathList);
public: /* get set */
	const MONSTERSTATE GetMonsterState() const { return m_MobState; }
	const Vector3 GetNextPos() const { return m_vNextPos; }
	const _int GetMonsterType() const { return m_iMonsterType; }
	const _float GetDistanceToPlayer() const { return m_fDistanceToPlayer; }
	_bool IsFoundPlayerFirstTime() const { return m_isFoundPlayerFistTime.load(); }

	void SetNextPos(const Vector3 _vNextPos) { this->m_vNextPos = _vNextPos; }
	void SetMonsterState(const MONSTERSTATE _MonsterState);
	void SetFoundPlayerFirstTime(_bool _isFindFirstTime);
protected:
	void UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange);
protected: /* get set*/
	SHPTR<AJobTimer> GetMonsterJobTimer() { return m_wpMonsterJobTimer.lock(); }
	const _float GetActiveRange() const { return m_fActiveRange; }
	const _float GetDeactiveRange() const { return m_fDeactiveRange; }
	const _float GetAttackRange() const { return m_fAttackRange; }

	void SetMonsterJobTimer(SHPTR<AJobTimer> _spJobTimer) { m_wpMonsterJobTimer = _spJobTimer; }
	void SetMonsterType(_int _iMonsterType) { this->m_iMonsterType = _iMonsterType; }
	void SetActiveRange(const _float _fActiveRange) { this->m_fActiveRange = _fActiveRange; }
	void SetDeactiveRange(const _float _fDeactiveRange) { this->m_fDeactiveRange = _fDeactiveRange; }
	void SetAttackRange(const _float _fAttackRange) { this->m_fAttackRange = _fAttackRange; }
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

	ATOMIC<_bool>							m_isFoundPlayerFistTime;

	_float												m_fActiveRange;
	_float												m_fDeactiveRange;
	_float												m_fAttackRange;
	ATOMIC<_float>							m_fDistanceToPlayer;
};


END