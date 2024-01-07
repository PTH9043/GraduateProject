#include "pch.h"
#include "Scene.h"
#include "Engine.h"


CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}






void CScene::BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,const ComPtr<ID3D12RootSignature>& _RootSignature)
{
	
	
	CTexturedModelShader* pShader = new CTexturedModelShader();
	pShader->CreateShader(L"..\\Resources\\Shader\\Shaders.hlsli", "VSStandard", "PSStandard", _Device, _RootSignature);
	pShader->CreateShaderVariables(_Device, _CommandList);
	m_ppObjects = CGameObject::LoadGeometryFromFile(_Device.Get(), _CommandList.Get(), _RootSignature.Get(), "Model/DB_five.bin", pShader);
	//m_ppObjects->SetShader(pShader);

}

void CScene::ReleaseObjects()
{
	
	if (m_ppObjects)
	{
		
		delete m_ppObjects;
	}
}

void CScene::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
	
			m_ppObjects->ReleaseUploadBuffers();
	}
}






void CScene::AnimateObjects(float fTimeElapsed)
{
	
		m_ppObjects->Animate(fTimeElapsed);
	
}


void CScene::Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera)
{
	pCamera->SetViewportsAndScissorRects(_CommandList);
	if (pCamera) pCamera->UpdateShaderVariables(_CommandList);
	
		if (m_ppObjects) m_ppObjects->Render(_CommandList.Get(), pCamera);
	
}

