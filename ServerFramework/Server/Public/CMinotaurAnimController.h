#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMINOTAURANIMCONTROLLER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMINOTAURANIMCONTROLLER_H

#include "AAnimController.h"

BEGIN(Server)
class CMinotaur;

class CMinotaurAnimController final : public AAnimController {
public:
	enum
	{
		MOB_RUN_STATE = MOB_STATE_END + 1, 
		MOB_BACK_STATE,
	};
public:
	CMinotaurAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix = _float4x4::Identity);
	DESTRUCTOR(CMinotaurAnimController)
public:
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
private:
	static constexpr _float RUNNING_DISTANCE{ 12 };

	WKPTR<CMinotaur>	m_wpMinotaur;
	_bool								m_isAttackMode;
	_bool								m_isRushMode;
	CUSTIMER						m_LastHitTimer;
	CUSTIMER						m_LastAttackTimer;
	CUSTIMER						m_IdleRandomValueChooseTimer;
	CUSTIMER						m_RushModeTimer;
	CUSTIMER						m_RushAttackTimer;
	CUSTIMER						m_HitCooldownTimer;
	CUSTIMER						m_ChangeStateTimer;
	_bool								m_isStartlastHitTime;
	_bool								m_isLastAttackWasFirst;
	_int									m_iRandomValue;
	Vector3							m_vRushTargetPos;
	_int									m_iRandomNumForHit;
	_int									m_iHitCount;
	
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMINOTAURANIMCONTROLLER_H