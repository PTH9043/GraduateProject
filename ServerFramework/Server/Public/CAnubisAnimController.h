#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CANUBISANIMCONTROLLER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CANUBISANIMCONTROLLER_H

#include "AAnimController.h"

BEGIN(Server)
class CAnubis;

class CAnubisAnimController final : public AAnimController {
public:
	enum 
	{
		MOB_SLEEP_STATE = MOB_STATE_END,
		MOB_AWAKE_STATE,
		MOB_TAUNT_STATE,
		MOB_HIT_STATE
	};
public:
	CAnubisAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix = _float4x4::Identity);
	DESTRUCTOR(CAnubisAnimController)
public:
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
private:
	WKPTR<CAnubis>		m_wpAnubis;

	_bool								m_isAttackMode;
	_bool								m_isAttackStart;
	_bool								m_isTauntMode;
	CUSTIMER						m_ShieldCoolTime;
	CUSTIMER						m_HitCooldownTimer;

	_int									m_iHitCount;
	_int									m_iRandomValueforAttack;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CANUBISANIMCONTROLLER_H
