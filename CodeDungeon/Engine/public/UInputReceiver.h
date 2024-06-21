#pragma once
#include "UComponent.h"

BEGIN(Engine)
using KEYCONTAINER = VECTOR<_bool>;

class UInputReceiver abstract : public UComponent{
public:
	UInputReceiver(CSHPTRREF<UDevice> _spDevice, _int _KeyContainerNUmber);
	UInputReceiver(const UInputReceiver& _rhs);
	DESTRUCTOR(UInputReceiver)
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override PURE;
	// Enable Keys
	_bool IsCheckEnableKey(_int _iIndex);

private:
	KEYCONTAINER	m_KeyContainer;
};

END

