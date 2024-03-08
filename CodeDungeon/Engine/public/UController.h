#pragma once

#include "UComponent.h"

BEGIN(Engine)
class UCharacter;

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

private:

};

END