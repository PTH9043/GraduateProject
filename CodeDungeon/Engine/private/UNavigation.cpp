#include "EngineDefine.h"
#include "UGameInstance.h"
#include "UNavigation.h"
#include "UTransform.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "UMethod.h"
#include "UCollider.h"
#include "UCell.h"
#include <fstream>

UNavigation::UNavigation(CSHPTRREF<UDevice> _spDevice)
	: UComponent(_spDevice),
	m_spCellContainer{},
	m_spCurCell{ nullptr },
	m_iCurIndex{ 0 }
{
}

UNavigation::UNavigation(const UNavigation& _rhs) : UComponent(_rhs),
m_spCellContainer{ _rhs.m_spCellContainer }
{
}

void UNavigation::Free()
{

	m_spCellContainer.reset();
}

HRESULT UNavigation::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	m_spCellContainer = Create<CELLCONTAINER>();

	UCollider::COLLIDERDESC tDesc;

	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(0.f, 0.f, 0.f);

	m_spCollider = std::static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_ABBCOLLIDER, { &tDesc}));

	RETURN_CHECK_FAILED(ReadyNeighbor(), E_FAIL);

	return S_OK;
}

//HRESULT UNavigation::NativeConstruct(const VECTOR<SHPTR<UVIBufferTerrain>>& _vecTerrain)
//{
//	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
//	RETURN_CHECK(0 == _vecTerrain.size(), E_FAIL);
//	using POSPOINTER = SHPTR<const VECTOR<_float3>>;
//
//	m_spCellContainer = std::make_shared<CELLCONTAINER>();
//	for (auto& iter : _vecTerrain)
//	{
//		POSPOINTER spTerrainPos{ nullptr };
//		const void* pIndices = iter->GetIndices();
//		const INDICIES32* pIndex32{ nullptr };
//		_uint iIndiescCnt = iter->GetIndexCnt();
//		{
//			spTerrainPos = iter->GetVertexPos();
//			RETURN_CHECK(nullptr == spTerrainPos, E_FAIL);
//		}
//		{
//			pIndex32 = static_cast<const INDICIES32*>(pIndices);
//			RETURN_CHECK(nullptr == pIndex32, E_FAIL);
//		}
//		for (_uint i = 0; i < iIndiescCnt; ++i) {
//			// Pos Container
//			ARRAY<_float3, UCell::POINT_END> PosContainer{
//				(*spTerrainPos.get())[pIndex32[i]._1], (*spTerrainPos.get())[pIndex32[i]._2], (*spTerrainPos.get())[pIndex32[i]._3]
//			};
//			// Create Constructor
//			SHPTR<UCell> pCell = CreateConstructorNative<UCell>(GetDevice(), PosContainer, i);
//			m_spCellContainer->push_back(pCell);
//		}
//	}
//	return S_OK;
//}

HRESULT UNavigation::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	NAVDESC tDesc = UMethod::ConvertTemplate_Index<NAVDESC>(_vecDatas, 0);
	m_iCurIndex = tDesc.iCurIndex;
	if ((*m_spCellContainer.get()).size() > m_iCurIndex)
		m_spCurCell = (*m_spCellContainer.get())[m_iCurIndex];
	else
		m_spCurCell = (*m_spCellContainer.get())[0];
	return S_OK;
}

const _float UNavigation::ComputeHeight(const _float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spCellContainer, false);

	if (nullptr != m_spCurCell) {
		return m_spCurCell->ComputeHeight(_vPosition);
	}
	else {
		_float3 vLine{};
		for (auto& iter : *m_spCellContainer.get()) {
			if (true == iter->IsIn(_vPosition, m_iCurIndex, vLine)) {
				m_spCurCell = iter;
				break;
			}
		}
		RETURN_CHECK(nullptr == m_spCurCell, _vPosition.y);
		return m_spCurCell->ComputeHeight(_vPosition);
	}
	return _vPosition.y;
}

void UNavigation::ComputeHeight(CSHPTRREF<UTransform> _spTransform)
{
	RETURN_CHECK(nullptr == _spTransform, ;);
	_float vValue = ComputeHeight(_spTransform->GetPos());

	_float3 vPos = _spTransform->GetPos();
	vPos.y = vValue;
	_spTransform->SetPos(vPos);
}

_bool UNavigation::IsMove(const _float3& _vPosition, SHPTR<UCell>& _spCell)
{
	RETURN_CHECK(nullptr == m_spCellContainer, false);

	if (-1 == m_iCurIndex || m_iCurIndex >= (*m_spCellContainer.get()).size()) {
		return false;
	}
	_float3 vLine{};
	_int iNeighBorIndex{ -1 };
	if (true == (*m_spCellContainer.get())[m_iCurIndex]->IsIn(_vPosition, iNeighBorIndex, vLine)) {
		m_spCurCell = (*m_spCellContainer.get())[m_iCurIndex];
		_spCell = m_spCurCell;
		return true;
	}
	else {
		if (0 <= iNeighBorIndex) {
			_uint iValue{ 0 };
			while (true) {
				RETURN_CHECK(-1 == iNeighBorIndex, false);

				if (true == (*m_spCellContainer.get())[iNeighBorIndex]->IsIn(_vPosition, iNeighBorIndex, vLine)) {
					m_iCurIndex = iNeighBorIndex;
					break;
				}
				++iValue;
				if (iValue >= 10) {
					FindCell(_vPosition);
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

SHPTR<UCell> UNavigation::FindCell(const _float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spCellContainer, nullptr);
	_int iNeighborIndex{ 0 };
	_float3 vLine{};
	for (auto& iter : (*m_spCellContainer.get())) {
		if (true == iter->IsIn(_vPosition, iNeighborIndex, vLine)) {
			m_iCurIndex = iter->GetIndex();
			return iter;
		}
	}
	return nullptr;
}

_bool UNavigation::IsCollision(SHPTR<UCollider>& _pCollider)
{
	if (nullptr == _pCollider || nullptr == m_spCollider)
		return false;

	if (true == m_spCollider->IsCollision(_pCollider))
		return true;

	return false;
}

void UNavigation::AddCell(SHPTR<UCell>& _spCell)
{
	if (nullptr == m_spCellContainer)
		return;

	m_spCellContainer->push_back(_spCell);
}

_bool UNavigation::Load(const _wstring& _wstrPath)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	std::ifstream load{ _wstrPath, std::ios::binary };
	RETURN_CHECK(load.fail(), false);

	_uint numCells;
	load.read(reinterpret_cast<char*>(&numCells), sizeof(_uint));

	m_spCellContainer->clear();
	_int iCellIndex = 0;
	for (_uint i = 0; i < numCells; ++i) {
		 UCell::CELLDECS tDesc;
		_float3 f3Color;

		load.read(reinterpret_cast<char*>(&f3Color), sizeof(f3Color));
		load.read(reinterpret_cast<char*>(&tDesc.vPoints), sizeof(tDesc.vPoints));
		load.read(reinterpret_cast<char*>(&tDesc.vNormal), sizeof(tDesc.vNormal));
		load.read(reinterpret_cast<char*>(&tDesc.vLine), sizeof(tDesc.vLine));
		load.read(reinterpret_cast<char*>(&tDesc.iNeighbor), sizeof(tDesc.iNeighbor));
		load.read(reinterpret_cast<char*>(&tDesc.iIndex), sizeof(tDesc.iIndex));

		SHPTR<UCell> NewCell = CreateConstructorNative<UCell>(spGameInstance->GetDevice(), tDesc);
		AddCell(NewCell);
	}

	RETURN_CHECK_FAILED(ReadyNeighbor(), false);

	return true;
}

_bool UNavigation::Save(const _wstring& _wstrPath)
{
	std::ofstream save{ _wstrPath, std::ios::binary };
	RETURN_CHECK(save.fail(), false);

	_uint numCells = static_cast<_uint>(m_spCellContainer->size());
	save.write(reinterpret_cast<const char*>(&numCells), sizeof(_uint));

	for (SHPTR<UCell> cellPtr : *m_spCellContainer) {
		UCell& cell = *cellPtr;
		save.write(reinterpret_cast<const char*>(&cell.GetColor()), sizeof(_float3));
		save.write(reinterpret_cast<const char*>(&cell.GetPoints()), sizeof(cell.GetPoints()));
		save.write(reinterpret_cast<const char*>(&cell.GetNormal()), sizeof(cell.GetNormal()));
		save.write(reinterpret_cast<const char*>(&cell.GetLines()), sizeof(cell.GetLines()));
		save.write(reinterpret_cast<const char*>(&cell.GetNeighbor()), sizeof(cell.GetNeighbor()));
		save.write(reinterpret_cast<const char*>(&cell.GetIndex()), sizeof(cell.GetIndex()));
	}
	return true;
}

HRESULT UNavigation::ReadyNeighbor()
{
	RETURN_CHECK(nullptr == m_spCellContainer, E_FAIL);

	for (auto& pSourCell : (*m_spCellContainer.get())) {
		for (auto& pDestCell : (*m_spCellContainer.get())) {
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(UCell::POINT_A), pSourCell->GetPoint(UCell::POINT_B))) {
				pSourCell->SetNeighbor(UCell::LINE_AB, pDestCell->GetIndex());
				continue;
			}

			if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(UCell::POINT_B), pSourCell->GetPoint(UCell::POINT_C))) {
				pSourCell->SetNeighbor(UCell::LINE_BC, pDestCell->GetIndex());
				continue;
			}

			if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(UCell::POINT_C), pSourCell->GetPoint(UCell::POINT_A)))
				pSourCell->SetNeighbor(UCell::LINE_CA, pDestCell->GetIndex());
		}
	}

	return S_OK;
}