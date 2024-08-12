#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMYANIMCONTROLLER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMYANIMCONTROLLER_H

#include "AAnimController.h"

BEGIN(Core)
END

BEGIN(Server)
class CMummy;

class CMummyAnimController final : public AAnimController {
public:
	enum MUMMYSTATE
	{
		MOB_SLEEP_STATE = MOB_STATE_END,
		MOB_AWAKE_STATE , 
		MOB_TAUNT_STATE,
		MOB_HIT_STATE
	};
public:
	CMummyAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix = _float4x4::Identity);
	DESTRUCTOR(CMummyAnimController)
public:
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
private:
	WKPTR<CMummy>	m_wpMummy;
	_bool								m_isTauntMode;
	_bool								m_isAttackMode;
	CUSTIMER						m_IdleTimer;
	CUSTIMER						m_IdleRandomValueChooseTimer;
	_bool								m_isStartlastHitTime;
	_bool								m_isLastAttackWasFirst;
	_int									m_iRandomValue;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMYANIMCONTROLLER_H