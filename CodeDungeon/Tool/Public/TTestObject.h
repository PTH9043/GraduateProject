#pragma once

#include "UPawn.h"

BEGIN(Engine)

class UVIBuffer;
class UShaderConstantBuffer;

END

BEGIN(Tool)

struct RECTCOLOR {
	_float4 g_RectColor;
};

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
	static constexpr _uint							RECTCOLOR_SIZE{ sizeof(RECTCOLOR) };
	// Cube
	SHPTR< UVIBuffer>								m_spVIBufferPlane;
	SHPTR< UShaderConstantBuffer>	m_spRectColorBuffer;
	RECTCOLOR											m_RectColor;
};

END