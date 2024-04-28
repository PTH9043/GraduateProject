#pragma once

#include "UComponent.h"

BEGIN(Engine)
class UCharacter;

/*
@ Date: 2024-04-28, Writer: 박태현
@ Explain
-  캐릭터의 움직임을 제어하는 클래스
*/
class UController abstract : public UComponent{
public:
	UController(CSHPTRREF<UDevice> _spDevice);
	UController(const UController& _rhs);
	virtual ~UController() = default;
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override PURE;
	// Tick
	virtual void Tick(UCharacter* _pCharacter, const _double& _dTimeDelta) PURE;
	// InputTriggerData
	virtual _bool InputTriggerData(UCharacter* _pCharacter, const _int _Data) PURE;
private:

};

END