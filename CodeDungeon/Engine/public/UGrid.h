#pragma once

#include "UPawn.h"

BEGIN(Engine)

class UVIBuffer;
class UShaderConstantBuffer;
class UParticle;

struct RECTCOLOR {
	_float4 g_RectColor;
};

class UGrid : public UPawn {
public:
	UGrid(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UGrid(const UGrid& _rhs);
	DESTRUCTOR(UGrid)
public:
	CLONE_MACRO(UGrid, "UGrid::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	CSHPTRREF<UCollider> GetCollider() { return m_spCollider; }
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
	SHPTR< UVIBuffer>								m_spVIBufferGrid;
	SHPTR<UCollider>								m_spCollider;
	//SHPTR<UParticle>		m_spParticle;
	SHPTR< UShaderConstantBuffer>	m_spRectColorBuffer;
	RECTCOLOR											m_RectColor;
};

END
