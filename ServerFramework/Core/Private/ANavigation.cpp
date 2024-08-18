#include "CoreDefines.h"
#include "ANavigation.h"
#include <fstream>
#include "ACell.h"
#include "ATransform.h"
#include "ACollider.h"

namespace Core {

	ANavigation::ANavigation() :
		m_CellContainer{  },
		m_spCurCell{ nullptr },
		m_iCurIndex{ 0 },
		m_spPrevCell{ nullptr },
		m_iPrevIndex{ 0 }
	{
	}

	ANavigation::ANavigation(const ANavigation& _rhs) : 
		m_CellContainer{  }, m_spCurCell{ nullptr },
		m_iCurIndex{ 0 },
		m_spPrevCell{ nullptr },
		m_iPrevIndex{ 0 }
	{
		m_CellContainer.reserve(_rhs.m_CellContainer.size());
		for (auto& iter : _rhs.m_CellContainer)
		{
			m_CellContainer.push_back(Create<ACell>(*iter.get()));
		}
		RETURN_CHECK(false == ReadyNeighbor(), ;);
	}

	void ANavigation::Free()
	{
		m_CellContainer.clear();
	}

	_bool ANavigation::NativeConstruct(const _string& _Paths)
	{
		return Load(_Paths);
	}

	void ANavigation::ComputeHeight(REF_OUT  Vector3& _vPosition)
	{
		RETURN_CHECK(0 == m_CellContainer.size(), ;);

		if (nullptr != m_spCurCell) {
			_vPosition.y = m_spCurCell->ComputeHeight(_vPosition);
		}
		else {
			Vector3 vLine{};
			SHPTR<ACell> closestCell = FindCell(_vPosition);

			if (nullptr != closestCell) {
				_vPosition.y = closestCell->ComputeHeight(_vPosition);
			}
		}
	}


	void ANavigation::ComputeHeight(CSHPTRREF<ATransform> _spTransform)
	{
		RETURN_CHECK(nullptr == _spTransform, ;);
		Vector3 vPos;
		ComputeHeight(REF_OUT vPos);
		_spTransform->SetPos(vPos);
	}

	_bool ANavigation::IsMove(const Vector3& _vPosition, REF_OUT SHPTR<ACell>& _spCell) {
		RETURN_CHECK(0 == m_CellContainer.size(), _vPosition.y);

		if (-1 == m_iCurIndex || m_iCurIndex >= (m_CellContainer).size()) {
			return false;
		}
		_int iNeighBorIndex{ -1 };
		if (true == m_CellContainer[m_iCurIndex]->IsIn(_vPosition, iNeighBorIndex)) {
			m_spPrevCell = m_spCurCell;
			m_spCurCell = m_CellContainer[m_iCurIndex];
			_spCell = m_spCurCell;
			return true;
		}
		else {
			if (0 <= iNeighBorIndex) {
				_uint iValue{ 0 };
				float minYDiff = -1.0f; // y 차이를 고려하기 위한 초기값
				SHPTR<ACell> closestCell{};
				bool found = false;

				while (true) {
					RETURN_CHECK(-1 == iNeighBorIndex, false);

					if (true == m_CellContainer[iNeighBorIndex]->IsIn(_vPosition, iNeighBorIndex)) {
						float yDiff = std::abs(_vPosition.y - m_CellContainer[iNeighBorIndex]->GetCenterPos().y);
						if (minYDiff == -1.0f || yDiff < minYDiff) {
							minYDiff = yDiff;
							closestCell = m_CellContainer[iNeighBorIndex];
							found = true;
						}
					}
					++iValue;
					if (iValue >= 10) {
						break;
					}
				}

				if (found) {
					m_spPrevCell = m_spCurCell;
					m_iPrevIndex = m_spCurCell->GetIndex();
					m_spCurCell = closestCell;
					m_iCurIndex = closestCell->GetIndex();
					_spCell = m_spCurCell;
					return true;
				}
				else {
					/*_spCell = FindCell(_vPosition);*/
					return false;
				}
			}
		}
		return false;
	}

	_bool ANavigation::IsMove(_int _iCurOnCellIndex, const Vector3& _vPosition, SHPTR<ACell>& _spCell)
	{
		m_iCurIndex = _iCurOnCellIndex;
		return IsMove(_vPosition, _spCell);
	}

	SHPTR<ACell> ANavigation::FindCell(const Vector3& _vPosition) {
		RETURN_CHECK(0 == m_CellContainer.size(), nullptr);

		_int iNeighborIndex{ 0 };
		Vector3 vLine{};
		_bool success = false;
		SHPTR<ACell> closestCell{};
		float minYDiff = -1.0f;

		_int range = 25;
		_int lowerBound = m_iCurIndex - range;
		_int upperBound = m_iCurIndex + range;

		while (lowerBound >= 0 || upperBound < m_CellContainer.size()) {
			if (lowerBound < 0) lowerBound = 0;
			if (upperBound > m_CellContainer.size()) upperBound = static_cast<_int>(m_CellContainer.size());

			for (_int i = lowerBound; i < upperBound; ++i) {
				auto& iter = m_CellContainer.at(i);
				if (true == iter->IsIn(_vPosition, iNeighborIndex)) {
					float yDiff = _vPosition.y - iter->GetHeightAtXZ(_vPosition.x, _vPosition.z);
					if (minYDiff == -1.0f || yDiff < minYDiff) {
						minYDiff = yDiff;
						m_spPrevCell = m_spCurCell;
						m_iCurIndex = iter->GetIndex();
						success = true;
						closestCell = iter;
						m_spCurCell = closestCell;
					}
				}
			}
			if (success) break;
			range += 25;
			lowerBound = m_iCurIndex - range;
			upperBound = m_iCurIndex + range;
		}

		if (success)
			return closestCell;
		else
			return nullptr;
	}

	SHPTR<ACell> ANavigation::FindCellWithoutUpdate(const Vector3& _vPosition) {
		RETURN_CHECK(0 == m_CellContainer.size(), nullptr);
		_int iNeighborIndex{ 0 };
		Vector3 vLine{};
		_bool success = false;
		SHPTR<ACell> closestCell{};
		float minYDiff = -1.0f;

		_int range = 25;
		_int lowerBound = m_iCurIndex - range;
		_int upperBound = m_iCurIndex + range;

		while (lowerBound >= 0 || upperBound < m_CellContainer.size()) {
			if (lowerBound < 0) lowerBound = 0;
			if (upperBound > m_CellContainer.size()) upperBound = static_cast<_int>(m_CellContainer.size());

			for (_int i = lowerBound; i < upperBound; ++i) {
				auto& iter = m_CellContainer.at(i);
				if (true == iter->IsIn(_vPosition, iNeighborIndex)) {
					float yDiff = _vPosition.y - iter->GetHeightAtXZ(_vPosition.x, _vPosition.z);
					if (minYDiff == -1.0f || yDiff < minYDiff) {
						minYDiff = yDiff;
						success = true;
						closestCell = iter;
					}
				}
			}
			if (success) break;
			range += 25;
			lowerBound = m_iCurIndex - range;
			upperBound = m_iCurIndex + range;
		}

		if (success)
			return closestCell;
		else
			return nullptr;
	}

	SHPTR<ACell> ANavigation::FindCell(const _int& _iIndex) {
		RETURN_CHECK(0 == m_CellContainer.size(), nullptr);
		_int iNeighborIndex{ 0 };
		Vector3 vLine{};
		_int range = 100;
		_int lowerBound = _iIndex - range;
		_int upperBound = _iIndex + range;

		while (lowerBound >= 0 || upperBound < m_CellContainer.size()) {
			if (lowerBound < 0) lowerBound = 0;
			if (upperBound > m_CellContainer.size()) upperBound = static_cast<_int>(m_CellContainer.size());

			for (_int i = lowerBound; i < upperBound; ++i) {
				auto& iter = m_CellContainer.at(i);
				if (iter->GetIndex() == _iIndex) {
					m_spPrevCell = m_spCurCell;
					m_iCurIndex = _iIndex;
					return iter;
				}
			}
			range += 100;
			lowerBound = _iIndex - range;
			upperBound = _iIndex + range;
		}
		return nullptr;
	}

	SHPTR<ACell> ANavigation::FindCellWithoutUpdate(const _int& _iIndex) {
		RETURN_CHECK(0 == m_CellContainer.size(), nullptr);
		_int iNeighborIndex{ 0 };
		Vector3 vLine{};

		for (auto& Cells : m_CellContainer)
		{
			if (Cells->GetIndex() == _iIndex) {
				return Cells;
			}
		}
		return nullptr;
	}

	SHPTR<ACell> ANavigation::FindCell(SHPTR<ACell> _spOtherCell)
	{
		RETURN_CHECK(0 == m_CellContainer.size(), nullptr);
		RETURN_CHECK(nullptr == _spOtherCell, nullptr);
		_int iNeighborIndex{ 0 };
		Vector3 vLine{};

		for (auto& Cells : m_CellContainer)
		{
			if (Cells->GetIndex() == _spOtherCell->GetIndex()) {
				m_iCurIndex = Cells->GetIndex();
				return Cells;
			}
		}
		return nullptr;
	}

	SHPTR<ACell> ANavigation::FindCellWithoutUpdate(SHPTR<ACell> _spOtherCell)
	{
		RETURN_CHECK(0 == m_CellContainer.size(), nullptr);
		RETURN_CHECK(nullptr == _spOtherCell, nullptr);
		_int iNeighborIndex{ 0 };
		Vector3 vLine{};

		for (auto& Cells : m_CellContainer)
		{
			if (Cells->GetIndex() == _spOtherCell->GetIndex()) {
				return Cells;
			}
		}
		return nullptr;
	}

	_bool ANavigation::IsCollision(SHPTR<ACollider>& _pCollider)
	{
		if (nullptr == _pCollider || nullptr == m_spCollider)
			return false;

		if (true == m_spCollider->IsCollision(_pCollider))
			return true;

		return false;
	}

	void ANavigation::AddCell(SHPTR<ACell>& _spCell)
	{
		if (0 == m_CellContainer.size())
			return;

		m_CellContainer.emplace_back(_spCell);
	}

	_bool ANavigation::Load(const _string& _strPath)
	{
		std::ifstream load{ _strPath, std::ios::binary };
		RETURN_CHECK(load.fail(), false);

		_uint numCells;
		load.read(reinterpret_cast<char*>(&numCells), sizeof(_uint));
		m_CellContainer.resize(numCells);
		
		for (auto& iter : m_CellContainer)
		{
			ACell::CELLDECS tDesc;
			Vector3 f3Color;

			load.read(reinterpret_cast<char*>(&f3Color), sizeof(f3Color));
			load.read(reinterpret_cast<char*>(&tDesc.vPoints), sizeof(tDesc.vPoints));
			load.read(reinterpret_cast<char*>(&tDesc.vNormal), sizeof(tDesc.vNormal));
			load.read(reinterpret_cast<char*>(&tDesc.vLine), sizeof(tDesc.vLine));
			load.read(reinterpret_cast<char*>(&tDesc.iNeighbor), sizeof(tDesc.iNeighbor));
			load.read(reinterpret_cast<char*>(&tDesc.iIndex), sizeof(_int));
			load.read(reinterpret_cast<char*>(&tDesc.bisJumpable), sizeof(tDesc.bisJumpable));

			SHPTR<ACell> NewCell = CreateInitNative<ACell>(tDesc);
			iter = NewCell;
		}

		RETURN_CHECK(false == ReadyNeighbor(), false);

		return true;
	}

	void ANavigation::SetCurCell(CSHPTRREF<ACell> newCell)
	{
		m_spCurCell = newCell; SetCurIndex(newCell->GetIndex());
	}

	_bool ANavigation::ReadyNeighbor()
	{
		RETURN_CHECK(0 == m_CellContainer.size(), false);

		for (auto& pSourCell : m_CellContainer) {
			for (auto& pDestCell : m_CellContainer) {

				if (pSourCell == pDestCell)
					continue;

				if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(ACell::POINT_A), pSourCell->GetPoint(ACell::POINT_B))) {
					pSourCell->SetNeighbor(ACell::LINE_AB, pDestCell);
					continue;
				}

				if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(ACell::POINT_B), pSourCell->GetPoint(ACell::POINT_C))) {
					pSourCell->SetNeighbor(ACell::LINE_BC, pDestCell);
					continue;
				}

				if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(ACell::POINT_C), pSourCell->GetPoint(ACell::POINT_A)))
					pSourCell->SetNeighbor(ACell::LINE_CA, pDestCell);
			}
		}
		return true;
	}

	Vector3 ANavigation::ClampPositionToCell(const Vector3& position)
	{
		// 현재 셀 내부에 있는 경우, 가장 가까운 선 위의 점을 찾아서 반환
		Vector3 closestPointOnEdges = m_spCurCell->GetClosestPointOnEdges(position);

		//현재 셀의 끝점에 도달하면 이웃 셀으로 변경
		for (auto& points : m_spCurCell->GetPoints())
		{
			if (closestPointOnEdges == points)
			{
				for (auto& NeighborIndex : m_spCurCell->GetNeighbor())
				{
					if (NeighborIndex != -1)
					{
						SHPTR<ACell> neighbor = FindCell(NeighborIndex);
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

	_float ANavigation::Heuristic(const Vector3& a, const Vector3& b)
	{
		Vector3 delta = { abs(b.x - a.x), abs(b.y - a.y), abs(b.z - a.z) };
		return delta.x + delta.y + delta.z;
	}

	PathFindingState ANavigation::StartPathFinding(const Vector3& start, const Vector3& end, CSHPTRREF<ACell> _startCell, CSHPTRREF<ACell> _endCell)
	{
		PathFindingState state;
		state.endCell = _endCell;

		SHPTR<ACell> startCell = _startCell;
		state.openSet.push({ startCell, 0, Heuristic(start, end), 0, nullptr });
		state.costSoFar[startCell] = 0;

		return state;
	}

	bool ANavigation::StepPathFinding(PathFindingState& state)
	{
		if (state.endCell == nullptr)
			return false;

		//for (int i = 0; i < 5; ++i)
		{
			if (state.openSet.empty()) {
				return true; // 경로를 찾지 못함
			}

			CellPathNode current = state.openSet.top();
			state.openSet.pop();

			if (current.cell == nullptr)
				return false;

			if (current.cell == state.endCell) {
				// 경로를 추적하여 반환
				state.isPathFound = true;
				for (SHPTR<ACell> c = state.endCell; c != nullptr; c = state.cameFrom[c]) {
					state.path.push_back(c);
				}
				std::reverse(state.path.begin(), state.path.end());
				return true; // 경로 찾음
			}

			for (int neighborIndex : current.cell->GetNeighbor()) {
				if (neighborIndex == -1) continue;
				SHPTR<ACell> neighbor = FindCellWithoutUpdate(neighborIndex);

				if (neighbor == nullptr)
					return false;

				_float newCost = state.costSoFar[current.cell] + Heuristic(current.cell->GetCenterPos(), neighbor->GetCenterPos());
				if (state.costSoFar.find(neighbor) == state.costSoFar.end() || newCost < state.costSoFar[neighbor]) {
					state.costSoFar[neighbor] = newCost;
					_float priority = newCost + Heuristic(neighbor->GetCenterPos(), state.endCell->GetCenterPos());
					state.openSet.push({ neighbor, newCost, Heuristic(neighbor->GetCenterPos(), state.endCell->GetCenterPos()), priority, current.cell });
					state.cameFrom[neighbor] = current.cell;
				}
			}
		}
		return false; // 아직 경로를 찾지 못함
	}

	VECTOR<Vector3> ANavigation::OptimizePath(const VECTOR<SHPTR<ACell>>& path, const Vector3& start, const Vector3& end)
	{
		VECTOR<Vector3> optimizedPath;
		optimizedPath.reserve(path.size() + 2);

		optimizedPath.push_back(start);

		for (const auto& cell : path) {
			optimizedPath.push_back(cell->GetCenterPos());
		}

		// Add end point
		optimizedPath.push_back(end);

		// Simplify the path
		VECTOR<Vector3> straightPath;
		straightPath.reserve(optimizedPath.size());

		Vector3 lastPoint = start;
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

	bool ANavigation::LineTest(const Vector3& start, const Vector3& end) {
		Vector3 direction = end - start;
		_float distance = static_cast<_float>(direction.Length());
		direction.Normalize();

		Vector3 currentPos = start;
		SHPTR<ACell> currentCell = FindCellWithoutUpdate(currentPos);

		if (!currentCell) {
			return false;
		}

		for (float t = 0; t < distance; t += 2.0f) {
			currentPos = start + direction * t;
			_int iNeighborIndex;

			if (!currentCell->IsIn(currentPos, iNeighborIndex)) {
				if (iNeighborIndex >= 0) {
					SHPTR<ACell> neighborCell = (m_CellContainer)[iNeighborIndex];
					if (neighborCell && neighborCell->IsIn(currentPos, iNeighborIndex)) {
						currentCell = neighborCell;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
		}
		return true;
	}

	SHPTR<ACell> ANavigation::ChooseRandomNeighborCell(int iterations)
	{
		RETURN_CHECK(nullptr == m_spCurCell, nullptr);

		SHPTR<ACell> currentCell = m_spCurCell;

		for (int i = 0; i < iterations; ++i) {
			const std::array<int, ACell::POINT_END>& neighbors = currentCell->GetNeighbor();
			std::vector<int> validNeighbors;

			// Collect all valid neighbor indices
			for (int neighborIndex : neighbors) {
				if (neighborIndex != -1) {
					validNeighbors.push_back(neighborIndex);
				}
			}
			RETURN_CHECK(validNeighbors.empty(), nullptr);

			// Seed the random number generator
			std::srand(static_cast<unsigned>(std::time(nullptr)));

			// Choose a random neighbor index
			int randomIndex = std::rand() % validNeighbors.size();

			SHPTR<ACell> neighborCell = FindCellWithoutUpdate(validNeighbors[randomIndex]);

			RETURN_CHECK(nullptr == neighborCell, nullptr);

			// Update currentCell to the chosen neighbor
			currentCell = neighborCell;
		}

		return currentCell;
	}
}