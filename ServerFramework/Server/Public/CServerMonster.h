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
	virtual void LateTick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual bool IsHit(APawn* _pPawn, const _double& _dTimeDelta) override;

	void TickUpdateBehavior(const _double _dTimeDelta, SHPTR<ASession> _spSession);
public: /* get set */
	_double GetElapsedTime() const { return m_dElapsedTime; }
	_double GetDeadTimeArcOpenEnd() const { return m_dDeadTimeArcOpenEnd; }

	void SetElapsedTime(_double _dElapsedTime) { m_dElapsedTime = _dElapsedTime; }
protected:
	void MoveAlongPath(const _double& _dTimeDelta);
	void FindPath(SHPTR<ANavigation> _spNavigation, SHPTR<ACell> _spSelfCell, SHPTR<ACell> _spTargetCell, const Vector3 _vSelfPos ,const Vector3 _vTargetPos);
protected:
	// Damaged
	virtual void Collision(APawn* _pPawn, const _double& _dTimeDelta) override;
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
};

END

#endif