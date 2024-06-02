
#pragma once
#include "UAnimationController.h"

BEGIN(Client)
class CMummy;
/*
@ Date: 2024-05-13, Writer: 이성현
@ Explain
-  Mummy를 조종하는 클래스
*/
class CMummyAnimController final : public UAnimationController {
public:
	CMummyAnimController(CSHPTRREF<UDevice> _spDevice);
	CMummyAnimController(const CMummyAnimController& _rhs);
	virtual ~CMummyAnimController() = default;

	enum MUMMYSTATE
	{
		ANIM_SLEEP = 6, ANIM_AWAKE = 7, ANIM_TAUNT = 8
	};
public:
	virtual void Free() override;
	CLONE_MACRO(CMummyAnimController, "CMummyAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	WKPTR< CMummy>			m_wpMummyMob;
};

END
