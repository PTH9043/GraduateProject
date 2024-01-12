#include "ToolScene.h"
#include "ToolGridObject.h"

void ToolScene::BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature)
{
	m_nObjects = 0;

	XMFLOAT3 xmf3Scale(6.0f, 1.0f, 6.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_pToolGrid = make_shared<ToolGridObject>(_Device, _CommandList, _RootSignature, 257, 257, 34, 34, xmf3Scale, xmf4Color);
}

void ToolScene::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}

void ToolScene::Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera)
{
	pCamera->SetViewportsAndScissorRects(_CommandList);
	if (pCamera) pCamera->UpdateShaderVariables(_CommandList);
	//씬을 렌더링하는 것은 씬을 구성하는 게임 객체(셰이더를 포함하는 객체)들을 렌더링하는 것이다.
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]) m_ppObjects[j]->Render(_CommandList, pCamera);
	}
	if (m_pToolGrid)
	{
		m_pToolGrid->Render(_CommandList, pCamera);
		m_pToolGrid->UpdateBoundingBox();
	}
}

