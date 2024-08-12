#pragma once
#include "CMonsterAnimController.h"
#include "USound.h"

BEGIN(Client)
class CMummy;
/*
@ Date: 2024-05-13, Writer: 이성현
@ Explain
-  Mummy를 조종하는 클래스
*/
class CNetworkMummyAnimController final : public CMonsterAnimController {
public:
	CNetworkMummyAnimController(CSHPTRREF<UDevice> _spDevice);
	CNetworkMummyAnimController(const CNetworkMummyAnimController& _rhs);
	virtual ~CNetworkMummyAnimController() = default;

	enum MUMMYSTATE
	{
		ANIM_SLEEP = 6, ANIM_AWAKE = 7, ANIM_TAUNT = 8
	};
public:
	virtual void Free() override;
	CLONE_MACRO(CNetworkMummyAnimController, "CNetworkMummyAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	WKPTR< CMummy>			m_wpMummyMob;

	//=====================SOUND CHANNEL================
	FMOD::Channel*		m_pAttack1Channel;
	FMOD::Channel*		m_pAttack2Channel;
	FMOD::Channel*		m_pSwhoosh1Channel;
	FMOD::Channel*		m_pSwhoosh2Channel;
	FMOD::Channel*		m_pTauntChannel;

	_int								m_iRandomNumforhit;
	FMOD::Channel*		m_pHitChannel;
	FMOD::Channel*		m_pGotHitChannel;

	_bool							m_isPlayAttackSound1;
	_bool							m_isPlayAttackSound2;
	_bool							m_isPlayHitSound;

};

END
