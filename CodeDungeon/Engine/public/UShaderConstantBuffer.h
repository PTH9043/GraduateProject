#pragma once
#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;
class UTableDescriptor;

/*
�� ���̴����� ���� �޶�� �ϴ� Buffer���� ShaderFile�� Bind�ϱ� ���� Ŭ������
�� ���̴��� �����ϱ� ���ؼ��� �ݵ�� �ش� ��ü�� �� �� �����ϰų� ������ ���� ���� �̿��Ѵ�.

���� ����ϴ� ������ _iNum ���� 1�̻� �־� �̸� �Ҵ� �� ���� Buffer�� ����ϰ�
���� ������� �ʰ� ���� �Ҵ� ������ �ʴ� �͵��� �׶� �׶����� ShaderConstantBuffer�� �Ҵ��Ѵ�.
*/
class UShaderConstantBuffer final : public UBase{
public:
	UShaderConstantBuffer();
	NO_COPY(UShaderConstantBuffer)
	DESTRUCTOR(UShaderConstantBuffer)
public:
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, 	const CBV_REGISTER& _eReg,   const _uint& _iSize, const _uint& _iNum = 1,const _bool& UseDefault=false);
	// Shader�� ���� �о� �ִ� �Լ��̴�. 
	HRESULT PushData(CSHPTRREF<UCommand> _spCommand,CSHPTRREF< UTableDescriptor> _spTableDescriptor, const void* _pBuffer, const _uint& _iSize);
private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptorHandle(const _uint& _iIndex);
private:
	// Use Upload
	ComPtr<Dx12Resource>						m_cpUploadBuffer;
	ComPtr<Dx12Resource>						m_cpDefaultBuffer;
	// Descriptor Heap
	ComPtr<Dx12DescriptorHeap>			m_cpCpuDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE	m_stD3DCpuDescriptorHandle;
	_int																m_iHandleIncrementSize;
	_bool m_bDataUpdated;
	_bool m_bUseDefaultBuffer;
	std::unique_ptr<_ubyte[]> m_pPreviousBuffer;

	BYTE* m_pMapBuffer;
	_uint															m_iElementSize;
	// CBV_REGISTER
	CBV_REGISTER										m_eCbvRegisterNumber;
	// elementIndex
	_uint															m_iShaderConstantBufferIndex;
	_int																m_iShaderConstantBufferCpuLocation;
	_uint															m_iElementNum;
};

END