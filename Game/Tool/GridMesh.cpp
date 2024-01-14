#include "GridMesh.h"

GridMesh::GridMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, 
	int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color) : CMesh(_Device, _CommandList)
{
	m_nVertices = nWidth * nLength;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_nWidth = nWidth;
	m_nLength = nLength;
	m_xmf3Scale = xmf3Scale;

	CDiffusedVertex* pVertices = new CDiffusedVertex[m_nVertices];

	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
		for (int x = xStart; x < (xStart + nWidth); x++, i++)
			pVertices[i] = CDiffusedVertex(XMFLOAT3((x * m_xmf3Scale.x), 0, (z * m_xmf3Scale.z)), xmf4Color);

	m_pd3dVertexBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices,
		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

	delete[] pVertices;

	m_nSubMeshes = 1;
	m_pnSubSetIndices.resize(m_nSubMeshes);
	m_ppnSubSetIndices.resize(m_nSubMeshes);

	m_ppd3dSubSetIndexBuffers.resize(m_nSubMeshes);
	m_ppd3dSubSetIndexUploadBuffers.resize(m_nSubMeshes);
	m_pd3dSubSetIndexBufferViews.resize(m_nSubMeshes);

	m_pnSubSetIndices[0] = ((nWidth * 2) * (nLength - 1)) + ((nLength - 1) - 1);
	m_ppnSubSetIndices[0].resize(m_pnSubSetIndices[0]);

	int j = 0;
	for (int z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			for (int x = 0; x < nWidth; x++)
			{
				if ((x == 0) && (z > 0)) m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
				m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
				m_ppnSubSetIndices[0][j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			for (int x = nWidth - 1; x >= 0; x--)
			{
				if (x == (nWidth - 1)) m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
				m_ppnSubSetIndices[0][j++] = (UINT)(x + (z * nWidth));
				m_ppnSubSetIndices[0][j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
	}

	m_ppd3dSubSetIndexBuffers[0] = Util::CreateBufferResource(
		_Device, _CommandList,
		m_ppnSubSetIndices[0].data(),
		sizeof(UINT) * m_pnSubSetIndices[0],
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&m_ppd3dSubSetIndexUploadBuffers[0]);

	m_pd3dSubSetIndexBufferViews[0].BufferLocation = m_ppd3dSubSetIndexBuffers[0]->GetGPUVirtualAddress();
	m_pd3dSubSetIndexBufferViews[0].Format = DXGI_FORMAT_R32_UINT;
	m_pd3dSubSetIndexBufferViews[0].SizeInBytes = sizeof(UINT) * m_pnSubSetIndices[0];
}

GridMesh::~GridMesh()
{

}

void GridMesh::Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	_CommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	_CommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);

	_CommandList->IASetIndexBuffer(&(m_pd3dSubSetIndexBufferViews[0]));
	_CommandList->DrawIndexedInstanced(m_pnSubSetIndices[0], 1, 0, 0, 0);
}
