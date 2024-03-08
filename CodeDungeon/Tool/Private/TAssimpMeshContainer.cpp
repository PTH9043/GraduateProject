#include "ToolDefines.h"
#include "TAssimpMeshContainer.h"
#include "TAssimpBoneNode.h"
#include "TAssimpModel.h"
#include "UMethod.h"

TAssimpMeshContainer::TAssimpMeshContainer(CSHPTRREF<UDevice> _spDevice)
	: UVIBuffer(_spDevice, VISPACE_TYPE::SPACE_3D, VIINSTANCE_TYPE::SINGLE),
	m_iMaterialIndex{ 0 },
	m_iNumBones{ 0 },
	m_iNumBuffers{ 0 },
	m_vecBoneNodes{},
	m_wstrName{ L"" },
	m_vecUVTexCoords{},
	m_vMinVertex{},
	m_vMaxVertex{}
{
}

TAssimpMeshContainer::TAssimpMeshContainer(const TAssimpMeshContainer& _rhs) :
	UVIBuffer(_rhs),
	m_iMaterialIndex{ _rhs.m_iMaterialIndex },
	m_iNumBones{ _rhs.m_iNumBones },
	m_iNumBuffers{ _rhs.m_iNumBuffers },
	m_vecBoneNodes{ _rhs.m_vecBoneNodes },
	m_wstrName{ _rhs.m_wstrName },
	m_vecUVTexCoords{ _rhs.m_vecUVTexCoords },
	m_vMinVertex{},
	m_vMaxVertex{}
{
}

void TAssimpMeshContainer::Free()
{
	if (false == IsClone()) {
		Make::ReleaseBuffer(m_pVertexData);
	}
}

HRESULT TAssimpMeshContainer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TAssimpMeshContainer::NativeConstruct(aiMesh* _pMesh, const _uint& _iType, CSHPTRREF<TAssimpModel> _pModel, const _xmmatrix& _mPivotMatrix)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	RETURN_CHECK(nullptr == _pMesh, E_FAIL);

	m_iMaterialIndex = _pMesh->mMaterialIndex;
	m_wstrName = UMethod::ConvertSToW(_pMesh->mName.data);

	if (TAssimpModel::TYPE::NONANIM == static_cast<TAssimpModel::TYPE>(_iType)) {
		RETURN_CHECK_FAILED(ReadyVertices(_pMesh, _pModel, _mPivotMatrix), E_FAIL);
	}
	else {
		RETURN_CHECK_FAILED(ReadyAnimVertices(_pMesh, _pModel), E_FAIL);
	}

	RETURN_CHECK_FAILED(ReadyIndices(_pMesh), E_FAIL);
	return S_OK;
}

HRESULT TAssimpMeshContainer::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	SHPTR<TAssimpModel> pModel = UMethod::ConvertTemplate_Index<SHPTR<TAssimpModel>>(_vecDatas, 0);
	RETURN_CHECK(nullptr == pModel, E_FAIL);

	VECTOR<SHPTR<TAssimpBoneNode>> BoneNodes{};
	for (auto& iter : m_vecBoneNodes) {
		_float4x4 OffsetMatrix = iter->GetOffsetMatrix();
		BoneNodes.push_back(pModel->GetBoneNode(iter->GetBoneName()));
	}
	m_vecBoneNodes.clear();
	m_vecBoneNodes = BoneNodes;
	return S_OK;
}

HRESULT TAssimpMeshContainer::SetUpBoneMatrices(VECTOR<_float4x4>& _vecMatrixces, const _float4x4& _mPivotMatrix)
{
	return S_OK;
}

void TAssimpMeshContainer::GetData(MESHDESC& _stMeshDesc)
{
	_stMeshDesc.iMatrialIndex = m_iMaterialIndex;
	_stMeshDesc.iNumVertices = GetVertexCnt();
	_stMeshDesc.iNumIndices = GetIndexCnt();
	_stMeshDesc.wstrName = m_wstrName;
	_stMeshDesc.pVtxModel = static_cast<VTXMODEL*>(m_pVertexData);
	_stMeshDesc.pIndices = static_cast<const INDICIES32*>(GetIndices());
	_stMeshDesc.pPosition = new _float3[GetVertexPos()->size()];
	_stMeshDesc.iBoneNodeCnt = (_uint)m_vecBoneNodes.size();
	for (auto& iter : m_vecBoneNodes)
		_stMeshDesc.BoneNodeNameList.push_back(iter->GetBoneName());
	_stMeshDesc.vMaxVertex = m_vMaxVertex;
	_stMeshDesc.vMinVertex = m_vMinVertex;
}

void TAssimpMeshContainer::GetData(ANIMMESHDESC& _stAnimMeshDesc)
{
	_stAnimMeshDesc.iMatrialIndex = m_iMaterialIndex;
	_stAnimMeshDesc.iNumVertices = GetVertexCnt();
	_stAnimMeshDesc.iNumIndices = GetIndexCnt();
	_stAnimMeshDesc.wstrName = m_wstrName;
	_stAnimMeshDesc.pVtxModel = static_cast<VTXANIMMODEL*>(m_pVertexData);
	_stAnimMeshDesc.pIndices = static_cast<const INDICIES32*>(GetIndices());
	_stAnimMeshDesc.iBoneNodeCnt = (_uint)m_vecBoneNodes.size();
	for (auto& iter : m_vecBoneNodes)
		_stAnimMeshDesc.BoneNodeNameList.push_back(iter->GetBoneName());
	_stAnimMeshDesc.vMaxVertex = m_vMaxVertex;
	_stAnimMeshDesc.vMinVertex = m_vMinVertex;
}

HRESULT TAssimpMeshContainer::ReadyVertices(aiMesh* _pMesh, CSHPTRREF<TAssimpModel> _spModel, const _xmmatrix& _mPivotMatrix)
{
	const _uint iNumVertices = _pMesh->mNumVertices;
	m_iNumBuffers = 1;

	VTXMODEL* pVertices = Make::AllocBuffer<VTXMODEL>(iNumVertices);
	ZeroMemory(pVertices, sizeof(VTXMODEL) * iNumVertices);

	VECTOR<_float3> pVerticesPos;
	pVerticesPos.resize(iNumVertices);

	for (_uint i = 0; i < iNumVertices; ++i) {
		// Position
		::memcpy(&pVertices[i].vPosition, &_pMesh->mVertices[i], sizeof(_float3));
		pVertices[i].vPosition = _float3::TransformCoord(pVertices[i].vPosition, _mPivotMatrix);
		::memcpy(&pVerticesPos[i], &pVertices[i].vPosition, sizeof(_float3));
		// Normal
		::memcpy(&pVertices[i].vNormal, &_pMesh->mNormals[i], sizeof(_float3));
		pVertices[i].vNormal = _float3::TransformNormal(pVertices[i].vNormal, _mPivotMatrix);
		// tangent
		if (nullptr != _pMesh->mTangents)
			::memcpy(&pVertices[i].vTangent, &_pMesh->mTangents[i], sizeof(_float3));
		// TextUv
		for (_uint j = 0; j < 5; ++j) {
			if (nullptr != _pMesh->mTextureCoords[j])
				::memcpy(&pVertices[i].vTexUV[j], &_pMesh->mTextureCoords[j][i], sizeof(_float2));
		}
	}

	m_iNumBones = _pMesh->mNumBones;
	for (_uint i = 0; i < m_iNumBones; ++i) {

		aiBone* pBone = _pMesh->mBones[i];
		_float4x4 OffsetMatrix{ _float4x4::Identity };
		::memcpy(&OffsetMatrix, &pBone->mOffsetMatrix, sizeof(_float4x4));

		SHPTR<TAssimpBoneNode> pBoneNode = _spModel->GetBoneNode(UMethod::ConvertSToW(pBone->mName.data));
		pBoneNode->SetOffsetMatrix(OffsetMatrix);
		m_vecBoneNodes.push_back(pBoneNode);
	}

	RETURN_CHECK_FAILED(CreateVtxBuffer(iNumVertices, sizeof(VTXMODEL), pVertices,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pVerticesPos), E_FAIL);

	m_pVertexData = static_cast<void*>(pVertices);
	return S_OK;
}

HRESULT TAssimpMeshContainer::ReadyAnimVertices(aiMesh* _pMesh, CSHPTRREF<TAssimpModel> _spModel)
{
	const _uint iNumVertices = _pMesh->mNumVertices;
	m_iNumBuffers = 1;

	VTXANIMMODEL* pVertices = Make::AllocBuffer<VTXANIMMODEL>(iNumVertices);
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * iNumVertices);

	VECTOR<_float3> pVerticesPos;
	pVerticesPos.resize(iNumVertices);

	for (_uint i = 0; i < iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &_pMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &_pMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &_pMesh->mTangents[i], sizeof(_float3));
		::memcpy(&pVerticesPos[i], &pVertices[i].vPosition, sizeof(_float3));

		for (_uint j = 0; j < 5; ++j) {
			if (nullptr != _pMesh->mTextureCoords[j])
				::memcpy(&pVertices[i].vTexUV[j], &_pMesh->mTextureCoords[j][i], sizeof(_float2));
		}
	}

	m_iNumBones = _pMesh->mNumBones;
	for (_uint i = 0; i < m_iNumBones; ++i) {

		aiBone* pBone = _pMesh->mBones[i];
		_float4x4 OffsetMatrix{ _float4x4::Identity };
		::memcpy(&OffsetMatrix, &pBone->mOffsetMatrix, sizeof(_float4x4));
		DirectX::XMStoreFloat4x4(&OffsetMatrix, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		SHPTR<TAssimpBoneNode> pBoneNode = _spModel->GetBoneNode(UMethod::ConvertSToW(pBone->mName.data));
		pBoneNode->SetOffsetMatrix(OffsetMatrix);
		m_vecBoneNodes.push_back(pBoneNode);

		for (_uint j = 0; j < pBone->mNumWeights; ++j)
		{
			if (0.0f == pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.x = pBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.y = pBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.z = pBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.w)
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.w = pBone->mWeights[j].mWeight;
			}
		}
	}

	RETURN_CHECK_FAILED(CreateVtxBuffer(iNumVertices, sizeof(VTXANIMMODEL), pVertices,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pVerticesPos), E_FAIL);

	m_pVertexData = static_cast<void*>(pVertices);
	return S_OK;
}

HRESULT TAssimpMeshContainer::ReadyIndices(aiMesh* _pMesh)
{
	const _uint NumPrimitive = _pMesh->mNumFaces;
	const _uint NumIndicesPerPrimitive = 3;

	// 16
	INDICIES32* pIndices = Make::AllocBuffer<INDICIES32>(NumPrimitive);
	for (_uint i = 0; i < NumPrimitive; ++i)
	{
		pIndices[i]._1 = _pMesh->mFaces[i].mIndices[0];
		pIndices[i]._2 = _pMesh->mFaces[i].mIndices[1];
		pIndices[i]._3 = _pMesh->mFaces[i].mIndices[2];
	}

	RETURN_CHECK_FAILED(CreateIndexBuffer(NumPrimitive, sizeof(INDICIES32), pIndices, DXGI_FORMAT_R32_UINT), E_FAIL);
	return S_OK;
}
