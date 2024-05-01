#pragma once
#include "UAnimationController.h"

BEGIN(Client)
class CWarriorPlayer;
/*
@ Date: 2024-04-30, Writer: 박태현
@ Explain
-  Warrior를 조종하는 클래스
*/
class CWarriorAnimController final : public UAnimationController {
public:
	CWarriorAnimController(CSHPTRREF<UDevice> _spDevice);
	CWarriorAnimController(const CWarriorAnimController& _rhs);
	virtual ~CWarriorAnimController() = default;
public:
	virtual void Free() override;
	CLONE_MACRO(CWarriorAnimController, "CWarriorAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	WKPTR< CWarriorPlayer>			m_wpWarriorPlayer;
	_wstring											m_wstrTrigger;
};

END
