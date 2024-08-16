#pragma once
#include "CMonsterAnimController.h"
#include "USound.h"

BEGIN(Client)
class CAnubis;
/*
@ Date: 2024-05-13, Writer: 이성현
@ Explain
-  Anubis를 조종하는 클래스
*/
class CNetworkAnubisAnimController final : public CMonsterAnimController {
public:
	CNetworkAnubisAnimController(CSHPTRREF<UDevice> _spDevice);
	CNetworkAnubisAnimController(const CNetworkAnubisAnimController& _rhs);
	virtual ~CNetworkAnubisAnimController() = default;

	enum AnubisSTATE
	{
		ANIM_SLEEP = 6, ANIM_AWAKE = 7, ANIM_TAUNT = 8
	};
public:
	virtual void Free() override;
	CLONE_MACRO(CNetworkAnubisAnimController, "CNetworkAnubisAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	WKPTR< CAnubis>			m_wpAnubisMob;

	_double				m_dTimerForFireCircle;

	//=====================SOUND CHANNEL================
	FMOD::Channel* m_pAttack1Channel;
	FMOD::Channel* m_pAttack2Channel;
	FMOD::Channel* m_pSwhoosh1Channel;
	FMOD::Channel* m_pSwhoosh2Channel;
	FMOD::Channel* m_pTauntChannel;

	_int								m_iRandomNumforhit;
	FMOD::Channel* m_pHitChannel;
	FMOD::Channel* m_pGotHitChannel;

	_bool							m_isPlayAttackSound1;
	_bool							m_isPlayAttackSound2;
	_bool							m_isPlayHitSound;
};

END
