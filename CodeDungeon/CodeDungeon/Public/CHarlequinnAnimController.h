
#pragma once
#include "UAnimationController.h"

BEGIN(Client)
class CHarlequinn;
/*
@ Date: 2024-07-05, Writer: 이성현
@ Explain
-  Minotaur를 조종하는 클래스
*/
class CHarlequinnAnimController final : public UAnimationController {
public:
	CHarlequinnAnimController(CSHPTRREF<UDevice> _spDevice);
	CHarlequinnAnimController(const CHarlequinnAnimController& _rhs);
	virtual ~CHarlequinnAnimController() = default;

	enum HARLEQUINNSTATE
	{
		ANIM_TAUNT = 6
	};

public:
	virtual void Free() override;
	CLONE_MACRO(CHarlequinnAnimController, "CHarlequinnAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;

private:
	WKPTR< CHarlequinn>			m_wpHarlequinnMob;

	_bool					m_bAttackMode;
	_double					m_dlastHitTime;
	_double					m_dlastAttackTime;
	_bool					m_bstartlastHitTime;
	_bool					m_blastAttackWasFirst;
	_double					m_dIdleTimer;
	_bool					m_bTauntMode;

	_double					m_didleRandomValueChoosingTimer;
	_int					m_iRandomValue;
};

END
