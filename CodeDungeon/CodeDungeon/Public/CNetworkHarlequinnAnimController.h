#pragma once
#include "CMonsterAnimController.h"
#include "USound.h"

BEGIN(Client)
class CHarlequinn;
/*
@ Date: 2024-07-05, Writer: 이성현
@ Explain
-  Minotaur를 조종하는 클래스
*/
class CNetworkHarlequinnAnimController final : public CMonsterAnimController {
public:
	CNetworkHarlequinnAnimController(CSHPTRREF<UDevice> _spDevice);
	CNetworkHarlequinnAnimController(const CNetworkHarlequinnAnimController& _rhs);
	virtual ~CNetworkHarlequinnAnimController() = default;

	enum HARLEQUINNSTATE
	{
		ANIM_TAUNT = 6
	};

public:
	virtual void Free() override;
	CLONE_MACRO(CNetworkHarlequinnAnimController, "CNetworkHarlequinnAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;

private:
	WKPTR< CHarlequinn>			m_wpHarlequinnMob;
	_float3										m_f3ThrowingPos;
	ARRAY<_float3, 6>					m_arrThrowingDir;
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
