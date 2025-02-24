#pragma once
#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;

/*
��� ���̴����� �������� ���̴� ��ü
������ �ϳ��� �����Ǿ� �Ѵ�.
Engine������ �����Ѵ�.
*/

class UGlobalConstantBuffer final : public UBase {
public:
	UGlobalConstantBuffer();
	NO_COPY(UGlobalConstantBuffer)
	DESTRUCTOR(UGlobalConstantBuffer)
public:
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, const CBV_REGISTER& _eReg, const _uint& _iSize, const _bool& UseDefault=false);

	void SettingGlobalData(CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize);
private:
	// Use Upload
	ComPtr<Dx12Resource>									m_cpUploadBuffer;
	ComPtr<Dx12Resource>						m_cpDefaultBuffer;
	// Descriptor Heap
	ComPtr<Dx12DescriptorHeap>						m_cpCpuDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE				m_cpCpuDescriptorHandle;
	BYTE* m_pMapBuffer;
	_uint																		m_iElementSize;
	// CBV_REGISTER
	_uint																		m_iCbvRegisterNumber;
	_bool								m_bUseDefaultBuffer;
	_bool								m_bCopiedUploadToDefaultOnce;
};

END

