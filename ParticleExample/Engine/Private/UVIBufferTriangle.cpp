#include "EngineDefines.h"
#include "UVIBufferTriangle.h"

UVIBufferTriangle::UVIBufferTriangle(CSHPTRREF<UDevice> _spDevice)
	: UVIBuffer(_spDevice, VISPACE_TYPE::SPACE_2D, VIINSTANCE_TYPE::SINGLE)
{
}

UVIBufferTriangle::UVIBufferTriangle(const UVIBufferTriangle& _rhs) : UVIBuffer(_rhs)
{
}

void UVIBufferTriangle::Free()
{
}

HRESULT UVIBufferTriangle::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	// Vertex 
	{
		// Vertex 개수
		const _uint VERTEX_CNT = 3;
		VECTOR<VTXDEFAULT>	Vertecies;
		Vertecies.resize(VERTEX_CNT);

		POSVECTOR VertexPos;
		VertexPos.resize(VERTEX_CNT);
		{
			// 1 -X, +Y                                                                                   
			Vertecies[0] = VTXDEFAULT{ _float3(-0.5f, -0.5f, 0.f), _float2(0.f, 1.f) };
			// 2 +X, +Y
			Vertecies[1] = VTXDEFAULT{ _float3(0.5f, -0.5f, 0.f), _float2(1.f, 1.f) };
			// 3 +X, -Y
			Vertecies[2] = VTXDEFAULT{ _float3(0.f, 0.5f, 0.f), _float2(0.5f, 0.f) };
		}
		// Position 복제
		for (_uint i = 0; i < VERTEX_CNT; ++i)
			VertexPos[i] = Vertecies[i].vPosition;

		RETURN_CHECK_FAILED(CreateVtxBuffer(VERTEX_CNT, sizeof(VTXDEFAULT), Vertecies.data(),
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, VertexPos), E_FAIL);
	}
	// Index
	{
		// Index 개수
		const _uint INDICES_CNT = 1;
		// 목록
		INDICIES16* pIndices = new INDICIES16[INDICES_CNT];
		ZeroMemory(pIndices, sizeof(INDICIES16) * INDICES_CNT);
		{
			pIndices[0] = { 0, 1, 2 };
		}
		RETURN_CHECK_FAILED(CreateIndexBuffer(INDICES_CNT,
			sizeof(INDICIES16), pIndices, DXGI_FORMAT_R16_UINT), E_FAIL);
	}
	return S_OK;
}

HRESULT UVIBufferTriangle::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	return __super::NativeConstructClone(_vecDatas);
}
