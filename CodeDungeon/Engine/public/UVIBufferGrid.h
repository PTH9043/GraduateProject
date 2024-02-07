#pragma once
#include "UVIBuffer.h"

BEGIN(Engine)

class  UVIBufferGrid : public UVIBuffer {
public:
	UVIBufferGrid(CSHPTRREF <UDevice> _spDevice, const VIBUFFERTYPE _eBufferType);
	UVIBufferGrid(const UVIBufferGrid& _rhs);
	DESTRUCTOR(UVIBufferGrid)
public:
	CLONE_THIS(UVIBufferGrid)
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
private:
};

END
