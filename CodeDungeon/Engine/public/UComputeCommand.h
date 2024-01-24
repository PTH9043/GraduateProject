#pragma once
#include "UCommand.h"

BEGIN(Engine)
class UDevice;

class UComputeCommand : public UCommand {
public:
	using ALLOCATORARR = ARRAY<ComPtr<Dx12CommandAllocator>, 5>;
public:
	UComputeCommand();
	NO_COPY(UComputeCommand)
		DESTRUCTOR(UComputeCommand)
public:
	virtual void Free() override;
	virtual HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, const ComPtr<Dx12CommandQueue>& _cpCommandQueue) override;
	virtual void WaitForSynchronization() override;
	// Begin Bind
	virtual void BindRootSignature(CSHPTRREF<URootSignature> _spRootSignature) override;
	virtual void Clear() override;
	/* Variable */
private:
	ALLOCATORARR		m_arrRenderAllocators;
	_uint							m_iAllocatorIndex;
};

END