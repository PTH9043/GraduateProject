#pragma once

#include "UPawn.h"

BEGIN(Engine)

class UVIBufferRect;

END

BEGIN(Tool)


class TTestObject : public UPawn{
public:
	TTestObject(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	TTestObject(const TTestObject& _rhs);
	DESTRUCTOR(TTestObject)
public:
	CLONE_MACRO(TTestObject, "TTestObject::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	// Cube
	SHPTR< UVIBufferRect>		m_spVIBufferRect;
};

END