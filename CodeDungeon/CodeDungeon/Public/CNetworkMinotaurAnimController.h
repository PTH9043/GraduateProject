#pragma once
#include "CMonsterAnimController.h"
#include "USound.h"

BEGIN(Client)
class CMinotaur;
/*
@ Date: 2024-07-05, Writer: 이성현
@ Explain
-  Minotaur를 조종하는 클래스
*/
class CNetworkMinotaurAnimController final : public CMonsterAnimController {
public:
	CNetworkMinotaurAnimController(CSHPTRREF<UDevice> _spDevice);
	CNetworkMinotaurAnimController(const CNetworkMinotaurAnimController& _rhs);
	virtual ~CNetworkMinotaurAnimController() = default;

public:
	virtual void Free() override;
	CLONE_MACRO(CNetworkMinotaurAnimController, "CNetworkMinotaurAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	WKPTR< CMinotaur>			m_wpMinotaurMob;

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
