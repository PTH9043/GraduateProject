#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H

#include "AMonster.h"

BEGIN(Server)

class CMummy final : public AMonster {
public:
	CMummy(OBJCON_CONSTRUCTOR, SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CMummy)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual bool IsHit(APawn* _pPawn, const _double& _dTimeDelta) override;
protected:
	// Damaged
	virtual void Collision(APawn* _pPawn, const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
private:
	MUMMYTYPE					m_eMumyType;
	_bool									m_isAttackMode;
	_bool									m_isLastAttackWasFirst;

	CUSTIMER							m_IdleTimer;
	CUSTIMER							m_IdleRandomValueChooseTimer;
	CUSTIMER							m_AttackTimer;
	_int										m_iRandomValue;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H