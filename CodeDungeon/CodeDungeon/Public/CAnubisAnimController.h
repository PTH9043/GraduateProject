
#pragma once
#include "CMonsterAnimController.h"

BEGIN(Client)
class CAnubis;
/*
@ Date: 2024-05-13, Writer: 이성현
@ Explain
-  Anubis를 조종하는 클래스
*/
class CAnubisAnimController final : public CMonsterAnimController {
public:
	CAnubisAnimController(CSHPTRREF<UDevice> _spDevice);
	CAnubisAnimController(const CAnubisAnimController& _rhs);
	virtual ~CAnubisAnimController() = default;

	enum AnubisSTATE
	{
		ANIM_SLEEP = 6, ANIM_AWAKE = 7, ANIM_TAUNT = 8
	};
public:
	virtual void Free() override;
	CLONE_MACRO(CAnubisAnimController, "CAnubisAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	WKPTR< CAnubis>			m_wpAnubisMob;

	_bool					m_bAttackMode;
	_bool					m_bAttackStart;

	_bool					m_bTauntMode;
	_double					m_dlastHitTime;
	_double					m_dlastAttackTime;
	_bool					m_bstartlastHitTime;
	_bool					m_blastAttackWasFirst;
	_double					m_dIdleTimer;
	_bool					m_bFoundPlayerFirsttime;
	_double					m_didleRandomValueChoosingTimer;
	_int					m_iRandomValue;

	_int				 m_iRandomValueforAttack;
	_double				m_dRecvAnimDuration;

};

END
