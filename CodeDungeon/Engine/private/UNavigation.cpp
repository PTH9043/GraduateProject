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
	m_spCellContainer->reserve(10000);

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
	RETURN_CHECK(nullptr == m_spCellContainer, _vPosition.y);

	if (nullptr != m_spCurCell) {
		return m_spCurCell->ComputeHeight(_vPosition);
	}
	else {
		_float3 vLine{};
		SHPTR<UCell> closestCell{};
		float minYDiff = -1.0f;

		for (auto& iter : *m_spCellContainer.get()) {
			if (true == iter->IsIn(_vPosition, m_iCurIndex, vLine)) {
				float yDiff = std::abs(_vPosition.y - iter->GetCenterPos().y);
				if (minYDiff == -1.0f || yDiff < minYDiff) {
					minYDiff = yDiff;
					closestCell = iter;
				}
			}
		}

		if (nullptr != closestCell) {
			m_spCurCell = closestCell;
			return m_spCurCell->ComputeHeight(_vPosition);
		}
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
			float minYDiff = -1.0f; // y 차이를 고려하기 위한 초기값
			SHPTR<UCell> closestCell{};
			bool found = false;

			while (true) {
				RETURN_CHECK(-1 == iNeighBorIndex, false);

				if (true == (*m_spCellContainer.get())[iNeighBorIndex]->IsIn(_vPosition, iNeighBorIndex, vLine)) {
					float yDiff = std::abs(_vPosition.y - (*m_spCellContainer.get())[iNeighBorIndex]->GetCenterPos().y);
					if (minYDiff == -1.0f || yDiff < minYDiff) {
						minYDiff = yDiff;
						closestCell = (*m_spCellContainer.get())[iNeighBorIndex];
						found = true;
					}
				}
				++iValue;
				if (iValue >= 10) {
					break;
				}
			}

			if (found) {
				m_iCurIndex = closestCell->GetIndex();
				m_spCurCell = closestCell;
				_spCell = m_spCurCell;
				return true;
			}
			else {
				FindCell(_vPosition);
				return false;
			}
		}
	}
	return false;
}


SHPTR<UCell> UNavigation::FindCell(const _float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spCellContainer, nullptr);
	_int iNeighborIndex{ 0 };
	_float3 vLine{};
	_bool success = false;
	SHPTR<UCell> closestCell{};
	float minYDiff = -1.0f; // -1.0f를 초기값으로 설정하여 첫 번째 유효한 셀의 y 차이로 초기화

	for (auto& iter : (*m_spCellContainer.get())) {
		if (true == iter->IsIn(_vPosition, iNeighborIndex, vLine)) {
			float yDiff = _vPosition.y - iter->GetHeightAtXZ(_vPosition.x, _vPosition.z);
			if (minYDiff == -1.0f || yDiff < minYDiff) { // 처음으로 유효한 값을 만났거나 현재의 y 차이가 최소값보다 작은 경우
				minYDiff = yDiff;
				m_iCurIndex = iter->GetIndex();
				success = true;
				closestCell = iter;
				m_spCurCell = closestCell;
			}
		}
	}

	if (success)
		return closestCell;
	else
		return nullptr;
}

SHPTR<UCell> UNavigation::FindCellWithoutUpdate(const _float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spCellContainer, nullptr);
	_int iNeighborIndex{ 0 };
	_float3 vLine{};
	_bool success = false;
	SHPTR<UCell> closestCell{};
	float minYDiff = -1.0f; // -1.0f를 초기값으로 설정하여 첫 번째 유효한 셀의 y 차이로 초기화

	for (auto& iter : (*m_spCellContainer.get())) {
		if (true == iter->IsIn(_vPosition, iNeighborIndex, vLine)) {
			float yDiff = _vPosition.y - iter->GetHeightAtXZ(_vPosition.x, _vPosition.z);
			if (minYDiff == -1.0f || yDiff < minYDiff) { // 처음으로 유효한 값을 만났거나 현재의 y 차이가 최소값보다 작은 경우
				minYDiff = yDiff;
				success = true;
				closestCell = iter;
			}
		}
	}

	if (success)
		return closestCell;
	else
		return nullptr;
}

SHPTR<UCell> UNavigation::FindCell(const _int& _iIndex)
{
	RETURN_CHECK(nullptr == m_spCellContainer, nullptr);
	_int iNeighborIndex{ 0 };
	_float3 vLine{};
	for (auto& iter : (*m_spCellContainer.get())) {
		if (iter->GetIndex() == _iIndex) {
			m_iCurIndex = _iIndex;
			return iter;
		}
	}
	return nullptr;
}

SHPTR<UCell> UNavigation::FindCellWithoutUpdate(const _int& _iIndex)
{
	RETURN_CHECK(nullptr == m_spCellContainer, nullptr);
	_int iNeighborIndex{ 0 };
	_float3 vLine{};
	for (auto& iter : (*m_spCellContainer.get())) {
		if (iter->GetIndex() == _iIndex) {
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

	m_spCellContainer->emplace_back(_spCell);
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
	_int test = 0;
	for (_uint i = 0; i < numCells; ++i) {
		 UCell::CELLDECS tDesc;
		_float3 f3Color;

		load.read(reinterpret_cast<char*>(&f3Color), sizeof(f3Color));
		load.read(reinterpret_cast<char*>(&tDesc.vPoints), sizeof(tDesc.vPoints));
		load.read(reinterpret_cast<char*>(&tDesc.vNormal), sizeof(tDesc.vNormal));
		load.read(reinterpret_cast<char*>(&tDesc.vLine), sizeof(tDesc.vLine));
		load.read(reinterpret_cast<char*>(&tDesc.iNeighbor), sizeof(tDesc.iNeighbor));
		load.read(reinterpret_cast<char*>(&tDesc.iIndex), sizeof(_int));
		load.read(reinterpret_cast<char*>(&tDesc.bisJumpable), sizeof(tDesc.bisJumpable));

		SHPTR<UCell> NewCell = CreateConstructorNative<UCell>(spGameInstance->GetDevice(), tDesc);
		AddCell(NewCell);
		iCellIndex++;
	}

	RETURN_CHECK_FAILED(ReadyNeighbor(), false);

	return true;
}

_bool UNavigation::Save(const _wstring& _wstrPath)
{
	std::ofstream save{ _wstrPath, std::ios::binary };
	RETURN_CHECK(save.fail(), false);
	RETURN_CHECK_FAILED(ReadyNeighbor(), false); 

	_uint numCells = static_cast<_uint>(m_spCellContainer->size());
	save.write(reinterpret_cast<const char*>(&numCells), sizeof(_uint));

	int i = 0;
	for (SHPTR<UCell> cellPtr : *m_spCellContainer) {
		UCell& cell = *cellPtr;
		save.write(reinterpret_cast<const char*>(&cell.GetColor()), sizeof(_float3));
		save.write(reinterpret_cast<const char*>(&cell.GetPoints()), sizeof(cell.GetPoints()));
		save.write(reinterpret_cast<const char*>(&cell.GetNormal()), sizeof(cell.GetNormal()));
		save.write(reinterpret_cast<const char*>(&cell.GetLines()), sizeof(cell.GetLines()));
		save.write(reinterpret_cast<const char*>(&cell.GetNeighbor()), sizeof(cell.GetNeighbor()));
		save.write(reinterpret_cast<const char*>(&cell.GetIndex()), sizeof(cell.GetIndex()));
		save.write(reinterpret_cast<const char*>(&cell.GetJumpableState()), sizeof(cell.GetJumpableState()));
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

_float3 UNavigation::ClampPositionToCell(const _float3& position)
{
	// 현재 셀 내부에 있는 경우, 가장 가까운 선 위의 점을 찾아서 반환
	_float3 closestPointOnEdges = m_spCurCell->GetClosestPointOnEdges(position);

	//현재 셀의 끝점에 도달하면 이웃 셀으로 변경
	for (auto& points : m_spCurCell->GetPoints())
	{
		if (closestPointOnEdges == points)
		{
			for (auto& NeighborIndex : m_spCurCell->GetNeighbor())
			{
				if(NeighborIndex != -1)
				{
					SHPTR<UCell> neighbor = FindCell(NeighborIndex);
					for (auto& points2 : neighbor->GetPoints())								
					{
						if (closestPointOnEdges == points2)
						{
							m_spCurCell = neighbor;
							return closestPointOnEdges;
						}

					}
				}
			}

		}
	}
	return closestPointOnEdges;
}

_float UNavigation::Heuristic(const _float3& a, const _float3& b)
{
	_float3 delta = { abs(b.x - a.x), abs(b.y - a.y), abs(b.z - a.z) };
	return delta.x + delta.y + delta.z;
}

UNavigation::PathFindingState UNavigation::StartPathFinding(const _float3& start, CSHPTRREF<UCell> _destCell)
{
	PathFindingState state;
	state.endCell = _destCell;

	SHPTR<UCell> startCell = FindCellWithoutUpdate(start);
	state.openSet.push({ startCell, 0, Heuristic(start, _destCell->GetCenterPos()), 0, nullptr });
	state.costSoFar[startCell] = 0;

	return state;
}

bool UNavigation::StepPathFinding(PathFindingState& state)
{
	if (state.openSet.empty()) {
		return true; // 경로를 찾지 못함
	}

	CellPathNode current = state.openSet.top();
	state.openSet.pop();

	if (current.cell == state.endCell) {
		// 경로를 추적하여 반환
		state.pathFound = true;
		for (SHPTR<UCell> c = state.endCell; c != nullptr; c = state.cameFrom[c]) {
			state.path.push_back(c);
		}
		std::reverse(state.path.begin(), state.path.end());
		return true; // 경로 찾음
	}

	for (int neighborIndex : current.cell->GetNeighbor()) {
		if (neighborIndex == -1) continue;
		SHPTR<UCell> neighbor = FindCellWithoutUpdate(neighborIndex);

		_float newCost = state.costSoFar[current.cell] + Heuristic(current.cell->GetCenterPos(), neighbor->GetCenterPos());
		if (state.costSoFar.find(neighbor) == state.costSoFar.end() || newCost < state.costSoFar[neighbor]) {
			state.costSoFar[neighbor] = newCost;
			_float priority = newCost + Heuristic(neighbor->GetCenterPos(), state.endCell->GetCenterPos());
			state.openSet.push({ neighbor, newCost, Heuristic(neighbor->GetCenterPos(), state.endCell->GetCenterPos()), priority, current.cell });
			state.cameFrom[neighbor] = current.cell;
		}
	}

	return false; // 아직 경로를 찾지 못함
}


VECTOR<_float3> UNavigation::OptimizePath(const VECTOR<SHPTR<UCell>>& path, const _float3& start, const _float3& end) {
	VECTOR<_float3> optimizedPath;

	// 시작점 추가
	optimizedPath.push_back(start);

	// 중간점 추가
	for (const auto& cell : path) {
		optimizedPath.push_back(cell->GetCenterPos());
	}

	// 끝점 추가
	optimizedPath.push_back(end);

	// 경로 최적화 (단순화)
	VECTOR<_float3> straightPath;
	_float3 lastPoint = start;
	straightPath.push_back(lastPoint);

	for (size_t i = 1; i < optimizedPath.size() - 1; ++i) {
		if (!LineTest(lastPoint, optimizedPath[i + 1])) {
			straightPath.push_back(optimizedPath[i]);
			lastPoint = optimizedPath[i];
		}
	}

	straightPath.push_back(end);
	return straightPath;
}

bool UNavigation::LineTest(const _float3& start, const _float3& end) {

	_float3 direction = end - start;
	_float distance = direction.Length();
	direction.Normalize();

	_float3 currentPos = start;
	for (float t = 0; t < distance; t += 1.0f) {
		currentPos = start + direction * t;
		SHPTR<UCell> TestCell = FindCellWithoutUpdate(currentPos);
		if (!TestCell) {
			return false;
		}
	}
	return true;
}

