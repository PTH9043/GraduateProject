#pragma once
#include "APawn.h"

BEGIN(Core)
class ASession;
class ACell;
class AJobTimer;

using ANIMFUNC = std::function<void*>(const _double&);

class CORE_DLL AMonster abstract : public APawn {
	using PATHCONTAINER = VECTOR<Vector3>;
public:
	AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(AMonster)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	void FindPlayer(SHPTR<ASession> _spSession);
	virtual void Tick(const _double& _dTimeDelta) PURE;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) PURE;
	virtual void ProcessPacket(_int _type, void* _pData) PURE;
	void InsertMobJobTimer(SESSIONID _PlayerID);
	_bool IsCanMove(const Vector3 _vPos);
	
public: /* get set */
	const MONSTERSTATE GetMonsterState() const { return m_MobState; }
	const _int GetMonsterType() const { return m_iMonsterType; }
	const _float GetDistanceToPlayer() const { return m_fDistanceToPlayer; }
	_bool IsFoundPlayerFirstTime() const { return m_isFoundPlayerFistTime; }
	const _bool IsCurrentAtkPlayer() const {return m_isCurrentAtkPlayer; }
	const _bool IsCurrentFindPlayer() const { return m_isCurrentFindPlayer; }
	const _bool IsCurrentNotFound() const { return m_isCurrentNotFound; }
	const SESSIONID& GetOwnerMonsterSessionID() const { return m_OwnerMonsterSessionID; }
	const Vector3 GetTargetPos() const { return m_vTargetPos; }

	void SetMonsterState(const MONSTERSTATE _MonsterState) {	m_MobState = _MonsterState;}
	void SetFoundPlayerFirstTime(_bool _isFindFirstTime) {m_isFoundPlayerFistTime = _isFindFirstTime;	}
	void SetOwnerMonsterSessionID(const SESSIONID& _OwnerMonsterSessionID) { this->m_OwnerMonsterSessionID = _OwnerMonsterSessionID; }
protected:
	void ComputeNextDir(const _double _dTimeDelta);
	void UpdateFindRange(const _float _fActiveRange, const _float _fDeactiveRange);
	void UpdateSelfStateToPlayerDistance(_bool _isCurAtkPlayer, _bool _isCurFindPlayer, _bool _isCurJustMove);
	void ResetTargetPlayer();
protected: /* get set*/
	SHPTR<AJobTimer> GetMonsterJobTimer() { return m_wpMonsterJobTimer.lock(); }
	const _float GetActiveRange() const { return m_fActiveRange; }
	const _float GetDeactiveRange() const { return m_fDeactiveRange; }
	const _float GetAttackRange() const { return m_fAttackRange; }
	SHPTR<ASession> GetTargetSession() const { return m_spTargetSession; }

	void SetMonsterJobTimer(SHPTR<AJobTimer> _spJobTimer) { m_wpMonsterJobTimer = _spJobTimer; }
	void SetMonsterType(_int _iMonsterType) { this->m_iMonsterType = _iMonsterType; }
	void SetActiveRange(const _float _fActiveRange) { this->m_fActiveRange = _fActiveRange ; }
	void SetDeactiveRange(const _float _fDeactiveRange) { this->m_fDeactiveRange = _fDeactiveRange ; }
	void SetAttackRange(const _float _fAttackRange) { this->m_fAttackRange = _fAttackRange ; }
	void SetCurrentAtkPlayer(const _bool _isCurrentAtkPlayer) { this->m_isCurrentAtkPlayer = _isCurrentAtkPlayer; }
	void SetCurrentFindPlayer(const _bool _isCurrentFindPlayer) {	m_isCurrentFindPlayer = _isCurrentFindPlayer; }
	void SetCurrentJustMove(const _bool _isCurrentJustMove) { m_isCurrentNotFound = _isCurrentJustMove; }
	void SetMoveSpeed(const _float _fMoveSpeed) { this->m_fMoveSpeed = _fMoveSpeed; }
	void SetTargetPos(const Vector3 _vTargetPos) { this->m_vTargetPos = _vTargetPos; }
private:
	virtual void Free() override;
private:
	ATOMIC<MONSTERSTATE>		m_MobState;
	_int													m_RoomIndex;
	_int													m_iMonsterType;

	_string											m_strAnimTriggerName;
	_string											m_strCurAnimName;

	WKPTR<AJobTimer>					m_wpMonsterJobTimer;

	std::atomic_bool							m_isFoundPlayerFistTime;
	std::atomic_bool							m_isCurrentAtkPlayer;
	std::atomic_bool							m_isCurrentFindPlayer;
	std::atomic_bool							m_isCurrentNotFound;

	_float												m_fActiveRange;
	_float												m_fDeactiveRange;
	_float												m_fAttackRange;
	_float												m_fMoveSpeed;
	ATOMIC<_float>							m_fDistanceToPlayer;
	SESSIONID									m_OwnerMonsterSessionID;

	SHPTR<ASession>						m_spTargetSession;
	Vector3											m_vTargetPos;
};


END