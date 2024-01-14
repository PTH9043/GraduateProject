#pragma once
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

    vector<XMFLOAT4>    m_pxmf4Colors;
    ComPtr<ID3D12Resource> m_pd3dColorBuffer;
    ComPtr<ID3D12Resource> m_pd3dColorUploadBuffer;
};
