#include "pch.h"
#include "Vertex.h"
#include "Mesh.h"


void CMesh::ReleaseUploadBuffers()
{
	if (m_pd3dPositionUploadBuffer) m_pd3dPositionUploadBuffer.Reset();
	if (m_pd3dNormalUploadBuffer) m_pd3dNormalUploadBuffer.Reset();
	if (m_ppd3dIndexUploadBuffers.data())
	{
		m_ppd3dIndexUploadBuffers.clear();
		vector<ComPtr<ID3D12Resource>>().swap(m_ppd3dIndexUploadBuffers);
	}
};

void CMesh::IASetBeforeRender(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	_CommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	_CommandList->IASetVertexBuffers(m_nSlot, m_pd3dVertexBufferViews.size(), m_pd3dVertexBufferViews.data());
}

void CMesh::RenderMesh(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT _nSubset)
{
	
	IASetBeforeRender(_CommandList);

	if ((m_nSubMeshes > 0) && (_nSubset < m_nSubMeshes))
	{
		_CommandList->IASetIndexBuffer(&m_pd3dIndexBufferViews[_nSubset]);
		_CommandList->DrawIndexedInstanced(m_pnSubSetIndices[_nSubset], 1, 0, 0, 0);
	}
	else
	{

		_CommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CMesh::RenderMesh_OneIndexBuffer(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{

	IASetBeforeRender(_CommandList);

	if (!m_ppd3dIndexBuffers.empty())
	{
		_CommandList->IASetIndexBuffer(&m_pd3dIndexBufferViews[0]);
		_CommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
	}
	else
	{
		_CommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}




































//
//CTriangleMesh::CTriangleMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) : CMesh(_Device, _CommandList)
//{
//	//삼각형 메쉬를 정의한다. 
//	m_nVertices = 3;
//	m_nStride = sizeof(CDiffusedVertex);
//	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	/*정점(삼각형의 꼭지점)의 색상은 시계방향 순서대로 빨간색, 녹색, 파란색으로 지정한다. RGBA(Red, Green, Blue,
//	Alpha) 4개의 파라메터를 사용하여 색상을 표현한다. 각 파라메터는 0.0~1.0 사이의 실수값을 가진다.*/
//	CDiffusedVertex pVertices[3];
//	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f,
//		1.0f));
//	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f,
//		1.0f));
//	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(Colors::Blue));
//	//삼각형 메쉬를 리소스(정점 버퍼)로 생성한다. 
//	m_pd3dVertexBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices, m_nStride* m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
//		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, & m_pd3dVertexUploadBuffer);
//		//정점 버퍼 뷰를 생성한다. 
//		m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
//		m_d3dVertexBufferView.StrideInBytes = m_nStride; //정점의 크기 바이트로 color정점이라 XMFLOAT4 ->16바이트 
//		m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices; //정점 버퍼 전체 크기
//}
//
//
//
//CCubeMeshDiffused::CCubeMeshDiffused(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, float fWidth, float fHeight, float fDepth) : CMesh(_Device,
//	_CommandList)
//{
//	//직육면체는 꼭지점(정점)이 8개이다.
//	m_nVertices = 8;
//	m_nStride = sizeof(CDiffusedVertex);
//	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
//	//정점 버퍼는 직육면체의 꼭지점 8개에 대한 정점 데이터를 가진다.
//	CDiffusedVertex pVertices[8];
//	pVertices[0] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), RANDOM_COLOR);
//	pVertices[1] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), RANDOM_COLOR);
//	pVertices[2] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), RANDOM_COLOR);
//	pVertices[3] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), RANDOM_COLOR);
//	pVertices[4] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), RANDOM_COLOR);
//	pVertices[5] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), RANDOM_COLOR);
//	pVertices[6] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), RANDOM_COLOR);
//	pVertices[7] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), RANDOM_COLOR);
//	m_pd3dVertexBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices,
//		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
//		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
//	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
//	m_d3dVertexBufferView.StrideInBytes = m_nStride;
//	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
//	/*인덱스 버퍼는 직육면체의 6개의 면(사각형)에 대한 기하 정보를 갖는다. 삼각형 리스트로 직육면체를 표현할 것이
//	므로 각 면은 2개의 삼각형을 가지고 각 삼각형은 3개의 정점이 필요하다. 즉, 인덱스 버퍼는 전체 36(=6*2*3)개의 인
//	덱스를 가져야 한다.*/
//	m_nIndices = 36;
//	UINT pnIndices[36];
//	//ⓐ 앞면(Front) 사각형의 위쪽 삼각형
//	pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
//	//ⓑ 앞면(Front) 사각형의 아래쪽 삼각형
//	pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
//	//ⓒ 윗면(Top) 사각형의 위쪽 삼각형
//	pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
//	//ⓓ 윗면(Top) 사각형의 아래쪽 삼각형
//	pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
//	//ⓔ 뒷면(Back) 사각형의 위쪽 삼각형
//	pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
//	//ⓕ 뒷면(Back) 사각형의 아래쪽 삼각형
//	pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
//	//ⓖ 아래면(Bottom) 사각형의 위쪽 삼각형
//	pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
//	//ⓗ 아래면(Bottom) 사각형의 아래쪽 삼각형
//	pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
//	//ⓘ 옆면(Left) 사각형의 위쪽 삼각형
//	pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
//	//ⓙ 옆면(Left) 사각형의 아래쪽 삼각형
//	pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
//	//ⓚ 옆면(Right) 사각형의 위쪽 삼각형
//	pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
//	//ⓛ 옆면(Right) 사각형의 아래쪽 삼각형
//	pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;
//	//인덱스 버퍼를 생성한다.
//	m_pd3dIndexBuffer = Util::CreateBufferResource(_Device, _CommandList, pnIndices,
//	sizeof(UINT)* m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER,
//		& m_pd3dIndexUploadBuffer);
//		//인덱스 버퍼 뷰를 생성한다. 
//	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
//		m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
//		m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
//}
//CCubeMeshDiffused::~CCubeMeshDiffused()
//{
//}