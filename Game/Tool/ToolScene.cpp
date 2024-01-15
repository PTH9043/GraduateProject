#include "ToolScene.h"
#include "ToolGridObject.h"
#include "CbvSrvUavDescriptorHeap.h"

void ToolScene::BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature)
{
	
	CreateCbvSrvDescriptorHeaps(0, 30);

	XMFLOAT3 xmf3Scale(8.0f, 1.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	shared_ptr<ToolGridObject> pToolGrid = make_shared<ToolGridObject>(_Device, _CommandList, _RootSignature, 257, 257, 34, 34, xmf3Scale, xmf4Color);
	shared_ptr<CDiffusedWireFrameShader> pGridShader = make_shared<CDiffusedWireFrameShader>(_Device, _RootSignature, L"Shader/cso/DiffuseVS.cso", L"Shader/cso/DiffusePS.cso");
	pToolGrid->SetShader(pGridShader);
	m_ppObjects.push_back(pToolGrid);
}


