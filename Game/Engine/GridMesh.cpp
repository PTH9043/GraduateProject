#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include "GridMesh.h"

GridMesh::GridMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
    m_nVertices = nWidth * nLength;
    m_nStride = sizeof(XMFLOAT3);
    m_nOffset = 0;
    m_nSlot = 0;
    m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

    m_nWidth = nWidth;
    m_nLength = nLength;
    m_xmf3Scale = xmf3Scale;

    m_pxmf3Positions.resize(m_nVertices);
    m_pxmf4Colors.resize(m_nVertices);

    for (int i = 0, z = zStart; z < (zStart + nLength); z++)
    {
        for (int x = xStart; x < (xStart + nWidth); x++, i++)
        {
            m_pxmf3Positions[i] = XMFLOAT3((x * m_xmf3Scale.x), 0, (z * m_xmf3Scale.z));
            m_pxmf4Colors[i] = xmf4Color;
        }
    }

    m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf3Positions.data(),
        m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

    m_pd3dColorBuffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf4Colors.data(),
       sizeof(XMFLOAT4) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dColorUploadBuffer);

    D3D12_VERTEX_BUFFER_VIEW positionBufferView;
    positionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
    positionBufferView.StrideInBytes = m_nStride;
    positionBufferView.SizeInBytes = m_nStride * m_nVertices;

    m_pd3dVertexBufferViews.push_back(positionBufferView);

    D3D12_VERTEX_BUFFER_VIEW colorBufferView;
    colorBufferView.BufferLocation = m_pd3dColorBuffer->GetGPUVirtualAddress();
    colorBufferView.StrideInBytes = sizeof(XMFLOAT4);
    colorBufferView.SizeInBytes = sizeof(XMFLOAT4) * m_nVertices;

    m_pd3dVertexBufferViews.push_back(colorBufferView);

    m_nSubMeshes = 1;
    m_pnSubSetIndices.resize(m_nSubMeshes);
    m_ppnSubSetIndices.resize(m_nSubMeshes);
    m_pnSubSetIndices[0] = ((nWidth * 2) * (nLength - 1)) + ((nLength - 1) - 1);
    m_ppnSubSetIndices[0].resize(m_pnSubSetIndices[0]);

    m_ppd3dIndexBuffers.resize(m_nSubMeshes);
    m_ppd3dIndexUploadBuffers.resize(m_nSubMeshes);
    m_pd3dIndexBufferViews.resize(m_nSubMeshes);

    int j = 0;
    for (int z = 0; z < nLength - 1; z++)
    {
        if ((z % 2) == 0)
        {
            for (int x = 0; x < nWidth; x++)
            {
                if ((x == 0) && (z > 0)) 
                    m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
                m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
                m_ppnSubSetIndices[0][j++] = (UINT)((x + (z * nWidth)) + nWidth);
            }
        }
        else
        {
            for (int x = nWidth - 1; x >= 0; x--)
            {
                if (x == (nWidth - 1)) 
                    m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
                m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
                m_ppnSubSetIndices[0][j++] = (UINT)((x + (z * nWidth)) + nWidth);
            }
        }
    }

    m_ppd3dIndexBuffers[0] = (Util::CreateBufferResource(
        _Device, _CommandList,
        m_ppnSubSetIndices[0].data(),
        sizeof(UINT) * m_pnSubSetIndices[0],
        D3D12_HEAP_TYPE_DEFAULT,
        D3D12_RESOURCE_STATE_INDEX_BUFFER,
        &m_ppd3dIndexUploadBuffers[0]));

    D3D12_INDEX_BUFFER_VIEW indexBufferView;
    indexBufferView.BufferLocation = m_ppd3dIndexBuffers[0]->GetGPUVirtualAddress();
    indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    indexBufferView.SizeInBytes = sizeof(UINT) * m_pnSubSetIndices[0];
    m_pd3dIndexBufferViews[0] = (indexBufferView);
}

GridMesh::~GridMesh()
{

}

