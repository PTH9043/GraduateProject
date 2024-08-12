#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERMONSTER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERMONSTER_H

#include "AMonster.h"

BEGIN(Core)
class ACell;
class ANavigation;
class ASession;
END

BEGIN(Server)

class CServerMonster abstract : public AMonster {
public:
	CServerMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CServerMonster)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
public: /* get set */
	const _double& GetElapsedTime() const { return m_dElapsedTime; }
	const _double& GetDeadAnimSpeed() const { return m_dDeadAnimSpeed; }
	const _double& GetDeadTimeArcOpenEnd() const { return m_dDeadTimeArcOpenEnd; }
	const _float GetCollisionNuckbackSpeed() const { return m_fCollisionNuckbackSpeed; }
	const _float GetApplySlidingMovementSpeed() const { return m_fApplySlidingMovementSpeed; }
	const Vector3& GetTargetToDir() const { return m_vTargetToDir; }
	const _string& GetRootName() const { return m_strRoomName; }

	void SetElapsedTime(_double _dElapsedTime) { m_dElapsedTime = _dElapsedTime; }
protected:
	virtual void TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession);
	virtual void MoveAlongPath(const _double& _dTimeDelta);
	void StartFindPath(SHPTR<ANavigation> _spNavigation, SHPTR<ACell> _spSelfCell, SHPTR<ACell> _spTargetCell, const Vector3 _vSelfPos ,const Vector3 _vTargetPos);
	void FindingPath(SHPTR<ANavigation> _spNavigation, const Vector3 _vSelfPos, const Vector3 _vTargetPos);
protected: /* get set */
	TWINTIMER& GetTimeAccumulator(REF_RETURN) { return m_TimeAccumulator; }
	_bool IsPathFinding() const { return m_isPathFinding; }
	PathFindingState& GetPathFindState(REF_RETURN) { return m_PathFindState; }
	const MOBSTATE& GetMobState() const { return m_MobState; }
	const _float GetRotSpeed() const { return m_fRotSpeed; }
private:
	virtual void Free() override;
private:
	TWINTIMER					m_TimeAccumulator;
	std::atomic_bool			m_isPathFinding;
	PathFindingState		m_PathFindState;
	VECTOR<Vector3>		m_AstarPath;
	ATOMIC<_int>				m_iNextPathIndex;
	ATOMIC<_bool>			m_isRecvPathFindState;
	_double							m_dDeadAnimSpeed;
	_double							m_dElapsedTime;
	_double							m_dDeadTimeArcOpenEnd;
	MOBSTATE					m_MobState;

	// RotateSpeed;
	_float								m_fRotSpeed;
	_float								m_fCollisionNuckbackSpeed;
	_float								m_fApplySlidingMovementSpeed;
	Vector3							m_vTargetToDir;
	_string							m_strRoomName;
};

END

#endif