#include "EngineDefine.h"
#include "UVIBufferGrid.h"

UVIBufferGrid::UVIBufferGrid(CSHPTRREF<UDevice> _spDevice, const VIBUFFERTYPE _eBufferType)
	: UVIBuffer(_spDevice, VISPACE_TYPE::SPACE_2D, VIINSTANCE_TYPE::SINGLE, _eBufferType)
{
}

UVIBufferGrid::UVIBufferGrid(const UVIBufferGrid& _rhs) :
	UVIBuffer(_rhs)
{
}

void UVIBufferGrid::Free()
{
}

HRESULT UVIBufferGrid::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	// Vertex 
	{
		// Vertex 개수
		const _uint VERTEX_CNT = 257 * 257;
		if (VIBUFFERTYPE::GENERIC == GetBufferType())
		{
			VECTOR<VTXDEFAULT>	Vertices;
			Vertices.resize(VERTEX_CNT);

			POSVECTOR VertexPos;

			VertexPos.resize(VERTEX_CNT);
			for (int i = 0, z = 0; z < 257; z++)
				for (int x = 0; x < (257); x++, i++)
					Vertices[i] = VTXDEFAULT{ _float3(x, 0, z), _float2(0.f, 0.f) };

			for (_uint i = 0; i < VERTEX_CNT; ++i)
				VertexPos[i] = Vertices[i].vPosition;

			RETURN_CHECK_FAILED(CreateVtxBuffer(VERTEX_CNT, sizeof(VTXDEFAULT), Vertices.data(),
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, VertexPos), E_FAIL);
		}
		else
		{
			VECTOR<VTXNORMAL>	Vertices;
			Vertices.resize(VERTEX_CNT);

			POSVECTOR VertexPos;

			VertexPos.resize(VERTEX_CNT);
			for (int i = 0, z = 0; z < 257; z++)
				for (int x = 0; x < (257); x++, i++)
					Vertices[i] = VTXNORMAL{ _float3(x, 0, z), _float3{0.f, 1.f, 0.f}, _float2(0.f, 0.f) };

			for (_uint i = 0; i < VERTEX_CNT; ++i)
				VertexPos[i] = Vertices[i].vPosition;

			RETURN_CHECK_FAILED(CreateVtxBuffer(VERTEX_CNT, sizeof(VTXDEFAULT), Vertices.data(),
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, VertexPos), E_FAIL);
		}
	}
	// Index
	{
		// Index 개수
		const _uint INDICES_CNT = ((257 * 2) * (257 - 1)) + ((257 - 1) - 1);
		// 목록
		INDICIES16* pIndices = new INDICIES16[INDICES_CNT];
		ZeroMemory(pIndices, sizeof(INDICIES16) * INDICES_CNT);
		{
			pIndices[0] = { 0, 1, 2 };
			pIndices[1] = { 0, 2, 3 };
		}

		//int j = 0;
		//for (int z = 0; z < 257 - 1; z++)
		//{
		//	if ((z % 2) == 0)
		//	{
		//		for (int x = 0; x < 257; x++)
		//		{
		//			if ((x == 0) && (z > 0))
		//				pIndices[j++] = { (x + (z * 257)),  (x + (z * 257)) + 257) } ;
		//			pIndices[j++] = (UINT)(x + (z * 257));
		//			pIndices[j++] = (UINT)((x + (z * 257)) + 257);
		//		}
		//	}
		//	else
		//	{
		//		for (int x = 257 - 1; x >= 0; x--)
		//		{
		//			if (x == (257 - 1))
		//				pIndices[j++] = (UINT)(x + (z * 257));
		//			pIndices[j++] = (UINT)(x + (z * 257));
		//			pIndices[j++] = (UINT)((x + (z * 257)) + 257);
		//		}
		//	}
		//}

		RETURN_CHECK_FAILED(CreateIndexBuffer(INDICES_CNT,
			sizeof(INDICIES16), pIndices, DXGI_FORMAT_R16_UINT), E_FAIL);
	}
	return S_OK;
}

HRESULT UVIBufferGrid::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	return __super::NativeConstructClone(_vecDatas);
}
