#include "EngineDefine.h"
#include "UComputeShader.h"
#include "UMethod.h"
#include "UGameInstance.h"
#include "URootSignature.h"
#include "UDevice.h"
#include "UTableDescriptor.h"
#include "UShaderConstantBuffer.h"
#include "UTexture.h"
#include "UShaderStructedBuffer.h"
#include "UComputeCommand.h"


UComputeShader::UComputeShader(CSHPTRREF<UDevice> _pDevice, CSHPTRREF<URootSignature> _spRootSignature, CSHPTRREF<UCommand> _spCommand, const SHADERDESC& _stDesc) :
	UShader(_pDevice, _spRootSignature, _spCommand, _stDesc)
{
}

UComputeShader::UComputeShader(const UComputeShader& _rhs)
	: UShader(_rhs)
{
}

void UComputeShader::SetTableDescriptor(CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	if (GetTableDescriptor() != _spTableDescriptor)
	{
		__super::SetTableDescriptor(_spTableDescriptor);
	}
}

void UComputeShader::Free()
{
}

HRESULT UComputeShader::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	return S_OK;
}

HRESULT UComputeShader::NativeConstructClone(const VOIDDATAS& _stDatas)
{
	return __super::NativeConstructClone(_stDatas);
}

HRESULT UComputeShader::BindUAVBuffer(const UAV_REGISTER _eReg, CSHPTRREF<UTexture> _spTexture)
{
	RETURN_CHECK(nullptr == _spTexture, E_FAIL);
	RETURN_CHECK(_eReg >= UAV_REGISTER::UAV_END, E_FAIL);
	RETURN_CHECK(_spTexture->GetTextureDescriptorType() != DESCRIPTOR_TYPE::UAV, E_FAIL);

	GetTableDescriptor()->BindUAV(_spTexture->GetShaderViewBeginHandle(DESCRIPTOR_TYPE::UAV), _eReg);
	return S_OK;
}

HRESULT	UComputeShader::BindUAVBuffer(const UAV_REGISTER _eReg, CSHPTRREF<UShaderStructedBuffer> _spStructedBuffer)
{
	RETURN_CHECK(nullptr == _spStructedBuffer, E_FAIL);
	RETURN_CHECK(_eReg >= UAV_REGISTER::UAV_END, E_FAIL);
	_spStructedBuffer->PushUAVData(GetTableDescriptor(), _eReg);
	return S_OK;
}

void UComputeShader::CommitLocalShaderDatas(CSHPTRREF<UCommand> _spCommand, const _uint& _iPosX, const _uint& _iPosY, const _uint& _iPosZ)
{
	GetTableDescriptor()->CommitComputeTable(_spCommand);

	_spCommand->GetGpuCmdList()->Dispatch(_iPosX, _iPosY, _iPosZ);
	_spCommand->WaitForSynchronization();
}

HRESULT UComputeShader::CreateShader()
{
	RETURN_CHECK(GetShaderDesc().ShaderList.size() != 1, E_FAIL);
	RETURN_CHECK(GetShaderDesc().eType != GRAPHICISCONTROLTYPE::COMPUTE, E_FAIL);

	D3D12_COMPUTE_PIPELINE_STATE_DESC		d3dComputePipeLineStateDesc;
	ZeroMemory(&d3dComputePipeLineStateDesc, sizeof(D3D12_COMPUTE_PIPELINE_STATE_DESC));

	SHADERMAININFO eMainInfo = GetShaderDesc().ShaderList.front();
	// ���̴� cso ������ �ε���
	_wstring wstrPath{ L"..\\Bin\\" };
	wstrPath += GetShaderDesc().wstrShaderName + L"_" + GetShaderType(eMainInfo) + L".cso";
	// ���̴� ������ �ҷ���
	ComPtr<DxBlob> ShaderFile{ UMethod::LoadBinary(wstrPath) };
	InsertShaderBlopContainer(eMainInfo, ShaderFile);
	// ByteCode�� ���� �ְ� PipeLine ���̴� ������ �°� ���� �ִ´�. 
	D3D12_SHADER_BYTECODE ByteCode{};
	ZeroMemory(&ByteCode, sizeof(D3D12_SHADER_BYTECODE));
	ByteCode = { ShaderFile->GetBufferPointer(), ShaderFile->GetBufferSize() };
	d3dComputePipeLineStateDesc.CS = ByteCode;
	d3dComputePipeLineStateDesc.pRootSignature = GetRootSignature()->GetRootSignature().Get();

	ComPtr<Dx12PipelineState> cpPipeLineState{ nullptr };
	// ���������� ����
	RETURN_CHECK_DXOBJECT(GetDevice()->GetDV()->CreateComputePipelineState(&d3dComputePipeLineStateDesc,
		IID_PPV_ARGS(&cpPipeLineState)), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	spGameInstance->AddPipeLineState(GetShaderDesc().wstrShaderName, cpPipeLineState);
	return S_OK;
}
