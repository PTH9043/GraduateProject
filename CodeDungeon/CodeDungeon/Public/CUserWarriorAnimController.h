#pragma once
#include "UAnimationController.h"

BEGIN(Client)
class CWarriorPlayer;
class UAnimation;
/*
@ Date: 2024-04-30, Writer: 박태현
@ Explain
-  Warrior를 조종하는 클래스
*/
class CUserWarriorAnimController final : public UAnimationController {
public:
	enum WARRIOR_STATE {
		ANIM_ROLL = 6, ANIM_RUN = 7, ANIM_JUMP = 8
	};
public:
	CUserWarriorAnimController(CSHPTRREF<UDevice> _spDevice);
	CUserWarriorAnimController(const CUserWarriorAnimController& _rhs);
	virtual ~CUserWarriorAnimController() = default;
public:
	virtual void Free() override;
	CLONE_MACRO(CUserWarriorAnimController, "CWarriorAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
	// 네트워크 정보를 받아오는 함수
	virtual void ReceiveNetworkProcessData(void* _pData) override;
private:
	WKPTR< CWarriorPlayer>			m_wpWarriorPlayer;
	_int													m_iWComboStack;
	_int													m_iSComboStack;
	_double											m_dTimeElapsed;
};

END
