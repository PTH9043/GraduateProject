#pragma once
#include "UController.h"

BEGIN(Engine)
class UAnimModel;
/*
@ Date: 2024-04-28, Writer: 박태현
@ Explain
-  애니메이션을 컨트롤하는 클래스
*/
class UAnimationController abstract : public UController {
public: 
	enum COMMONSTATE
	{
		ANIM_IDLE, ANIM_MOVE, ANIM_ATTACK, ANIM_COMBO, ANIM_RUN
	};
public:
	UAnimationController(CSHPTRREF<UDevice> _spDevice);
	UAnimationController(const UAnimationController& _rhs);
	DESTRUCTOR(UAnimationController)
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override PURE;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override PURE;
public: /* get set*/
	const _int GetAnimState() const { return m_iAnimState;  }
protected: /* get set  */
	void SetAnimState(const _int _iAnimState) { this->m_iAnimState = _iAnimState; }
private:
	_int			m_iAnimState;
};

END