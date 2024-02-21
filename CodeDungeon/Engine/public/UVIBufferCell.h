#pragma once
#include "UVIBuffer.h"

BEGIN(Engine)
/*
@ Date: 2024-02-019, Writer:이성현
@ Explain
- Cell을 위한 VI버퍼
*/


class  UVIBufferCell : public UVIBuffer {
public:
	UVIBufferCell(CSHPTRREF <UDevice> _spDevice, const VIBUFFERTYPE _eBufferType);
	UVIBufferCell(const UVIBufferCell& _rhs);
	DESTRUCTOR(UVIBufferCell)
public:
	CLONE_THIS(UVIBufferCell)
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	HRESULT NativeConstruct(ARRAY<_float3, 3>& _Points);
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
private:
};

END