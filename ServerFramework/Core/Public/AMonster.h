#pragma once
#include "APawn.h"

BEGIN(Core)
class ASession;
class ACell;
class AJobTimer;

using ANIMFUNC = std::function<void*>(const _double&);

class CORE_DLL AMonster abstract : public APawn {
	using PATHCONTAINER = CONSTACK<Vector3>;
public:
	AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(AMonster)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	void FindPlayer(SHPTR<ASession> _spSession);
	virtual void Tick(const _double& _dTimeDelta) PURE;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) PURE;
	void InsertMobJobTimer(_int _PlayerID);

	_bool IsChangeTargetPlayer(SESSIONID _TargetPlayerID, _float _fDistance);
public: /* get set */
	const MONSTERSTATE GetMonsterState() const { return m_MobState; }
	const Vector3 GetNextPos() const { return m_vNextPos; }
	const _int GetMonsterType() const { return m_iMonsterType; }
	const _float GetDistanceToPlayer() const { return m_fDistanceToPlayer; }
	_bool IsFoundPlayerFirstTime() const { return m_isFoundPlayerFistTime; }
	const SESSIONID GetCurrentTargetPlayerID() const { return m_CurrentTargetPlayerID; }
	const _bool IsCurrentFindPlayer() const { return m_isCurrentFindPlayer; }

	void SetNextPos(const Vector3 _vNextPos) { this->m_vNextPos = _vNextPos; }
	void SetMonsterState(const MONSTERSTATE _MonsterState);
	void SetFoundPlayerFirstTime(_bool _isFindFirstTime);
	void InsertPathList(LIST<Vector3>&& _PathList);
protected:
	void UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange);
	void MoveToNextPos(const _double& _dTimeDelta);
protected: /* get set*/
	SHPTR<AJobTimer> GetMonsterJobTimer() { return m_wpMonsterJobTimer.lock(); }
	const _float GetActiveRange() const { return m_fActiveRange; }
	const _float GetDeactiveRange() const { return m_fDeactiveRange; }
	const _float GetAttackRange() const { return m_fAttackRange; }
	const PATHCONTAINER& GetPathList() const { return m_PathList; }

	void SetMonsterJobTimer(SHPTR<AJobTimer> _spJobTimer) { m_wpMonsterJobTimer = _spJobTimer; }
	void SetMonsterType(_int _iMonsterType) { this->m_iMonsterType = _iMonsterType; }
	void SetActiveRange(const _float _fActiveRange) { this->m_fActiveRange = _fActiveRange; }
	void SetDeactiveRange(const _float _fDeactiveRange) { this->m_fDeactiveRange = _fDeactiveRange; }
	void SetAttackRange(const _float _fAttackRange) { this->m_fAttackRange = _fAttackRange; }
	void SetTargetPlayerID(const SESSIONID _SessionID);
	void SetCurrentFindPlayer(const _bool _isFindPlayer);
	void SetMoveSpeed(const _float _fMoveSpeed) { this->m_fMoveSpeed = _fMoveSpeed; }
private:
	virtual void Free() override;
private:
	// 가야할 다음 장소
	Vector3											m_vNextPos;
	ATOMIC<MONSTERSTATE>		m_MobState;
	_int													m_RoomIndex;
	_int													m_iMonsterType;
	ATOMIC<SESSIONID>					m_CurrentTargetPlayerID;

	PATHCONTAINER							m_PathList;

	_string											m_strAnimTriggerName;
	_string											m_strCurAnimName;

	WKPTR<AJobTimer>					m_wpMonsterJobTimer;

	std::atomic_bool							m_isFoundPlayerFistTime;
	std::atomic_bool							m_isCurrentFindPlayer;

	_float												m_fActiveRange;
	_float												m_fDeactiveRange;
	_float												m_fAttackRange;
	_float												m_fMoveSpeed;
	ATOMIC<_float>							m_fDistanceToPlayer;
};


END