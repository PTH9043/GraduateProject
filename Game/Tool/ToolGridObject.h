#pragma once
#include "ToolDefines.h"
#include "GameObject.h"
#include "GridMesh.h"

class ToolGridObject : public CGameObject
{
public:
	ToolGridObject(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
	~ToolGridObject();
private:
	int								m_nWidth;
	int								m_nLength;
	XMFLOAT3						m_xmf3Scale;

	vector<std::shared_ptr<CMesh>>	m_ppMeshes;
	int m_nMeshes;

public:
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
	virtual void SetMesh(int nIndex, const shared_ptr<CMesh> pMesh);
	virtual void Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera);
};

