#pragma once
#include "ToolDefines.h"
#include "Mesh.h"

class GridMesh : public CMesh
{
public:
	GridMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~GridMesh();

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }


protected:
    int m_nWidth;
    int m_nLength;
    XMFLOAT3 m_xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

    int m_nSubMeshes = 0;
    vector<int> m_pnSubSetIndices;
    vector<vector<UINT>> m_ppnSubSetIndices;

    vector<ComPtr<ID3D12Resource>> m_ppd3dSubSetIndexBuffers;
    vector<ComPtr<ID3D12Resource>> m_ppd3dSubSetIndexUploadBuffers;
    vector<D3D12_INDEX_BUFFER_VIEW> m_pd3dSubSetIndexBufferViews;

public:
	virtual void Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
};

