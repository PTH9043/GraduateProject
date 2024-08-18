#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CHARLEQUINANIMCONTROLLER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CHARLEQUINANIMCONTROLLER_H

#include "AAnimController.h"

BEGIN(Server)
class CHarlequin;

class CHarlequinAnimController final : public AAnimController {
public:
	enum MUMMYSTATE
	{
		MOB_TAUNT_STATE = MOB_STATE_END,
		MOB_JUMP_STATE,
	};
public:
	CHarlequinAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix = _float4x4::Identity);
	DESTRUCTOR(CHarlequinAnimController)
public:
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
private:
	WKPTR<CHarlequin>		m_wpHarlequin;
	_bool									m_isTauntMode;
	_bool									m_isAttackMode;
	CUSTIMER							m_LastHitTimer;
	CUSTIMER							m_LastAttackTimer;
	CUSTIMER							m_IdleTimer;
	CUSTIMER							m_Attack1FirstTime;
	CUSTIMER							m_IdleRandomValueChoosingTimerForPartrol;
	CUSTIMER							m_HitCoolTimer;
	CUSTIMER							m_JumpTimer;
	CUSTIMER							m_RelaxTimer;
	_bool									m_isStartLastHitTime;
	_bool									m_isLastAttackWasFirst;
	Vector3								m_vThrowingPos;
	ARRAY<Vector3, 6>		m_arrThrowindDir;
	_int										m_iRandomValueForPatrol;
	_int										m_iRandomValueForDoge;
	_bool									m_isDoge;
	_bool									m_isWillWalkUntilCloseRange;
	_bool									m_isWillJumpToCloseRange;
	_bool									m_isHitCooldown;
	_int										m_iHitCount;
	_bool									m_isJumpEnable;
	_int										m_iJumpCounter;
	_int										m_iMaxJumpCount;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CHARLEQUINANIMCONTROLLER_H
