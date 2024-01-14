#include "ToolScene.h"
#include "ToolGridObject.h"
#include "CbvSrvUavDescriptorHeap.h"

void ToolScene::BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature)
{
	m_pDescriptorHeap = make_unique<CbvSrvUavDescriptorHeap>();
	CreateCbvSrvDescriptorHeaps(_Device, 0, 30);

	XMFLOAT3 xmf3Scale(6.0f, 1.0f, 6.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_pToolGrid = make_shared<ToolGridObject>(_Device, _CommandList, _RootSignature, 257, 257, 34, 34, xmf3Scale, xmf4Color);
}


void ToolScene::Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera)
{
	if (m_pDescriptorHeap->m_pd3dCbvSrvUavDescriptorHeap)_CommandList->SetDescriptorHeaps(1, m_pDescriptorHeap->m_pd3dCbvSrvUavDescriptorHeap.GetAddressOf());

	pCamera->SetViewportsAndScissorRects(_CommandList);
	pCamera->UpdateShaderVariables(_CommandList);

	if (m_pToolGrid)
	{
		m_pToolGrid->Render(_CommandList, pCamera);
		m_pToolGrid->UpdateBOB();
	}
}

