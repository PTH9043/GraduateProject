#include "CoreDefines.h"
#include "APathFinder.h"
#include "ANavigation.h"
#include "ACell.h"
#include "ANavigation.h"
#include "ATransform.h"

namespace Core {

	APathFinder::APathFinder() : 
		m_MaxCellCount{0}, m_CheckOpens{}, m_DistanceWeights{}
		,m_VisitedListes{},  m_spNavigation{nullptr}
	{
	}

	_bool APathFinder::NativeConstruct(SHPTR<ANavigation> _spNavigation)
	{
		m_spNavigation = _spNavigation;
		// Cells
		m_MaxCellCount = static_cast<_int>(_spNavigation->GetCells()->size());
		// value 
		m_CheckOpens.resize(m_MaxCellCount);
		m_DistanceWeights.resize(m_MaxCellCount);
		return true;
	}

	LIST<SHPTR<ACell>> APathFinder::FindPath(Vector3 _vStartPos, Vector3 _vEndPos)
	{
		Release();
		SHPTR<ACell> spStartCell = m_spNavigation->FindCell(_vStartPos);
		SHPTR<ACell> spEndCell = m_spNavigation->FindCell(_vEndPos);
		RETURN_CHECK(nullptr == spStartCell || nullptr == spEndCell, LIST<SHPTR<ACell>>{});
		MakeRoutine(spStartCell, spEndCell);
		m_SavePathList = OptimizePath(m_SavePathList, spStartCell, spEndCell);
		return m_SavePathList;
	}

	LIST<SHPTR<ACell>> APathFinder::FindPath(SHPTR<ATransform> _spStartTr, SHPTR<ATransform> _spEndTr)
	{
		return FindPath(_spStartTr->GetPos(), _spEndTr->GetPos());
	}

	LIST<SHPTR<ACell>> APathFinder::OptimizePath(const LIST<SHPTR<ACell>>& _path, SHPTR<ACell> _start, SHPTR<ACell> _end)
	{
		VECTOR<SHPTR<ACell>> optimizedPath;
		optimizedPath.reserve(_path.size() + 2);

		optimizedPath.push_back(_start);
		for (const auto& cell : _path) {
			optimizedPath.push_back(cell);
		}
		// Add end point
		optimizedPath.push_back(_end);

		// Simplify the path
		LIST<SHPTR<ACell>> straightPath;
		SHPTR<ACell> lastPoint = _start;
		straightPath.push_back(lastPoint);
		for (size_t i = 1; i < optimizedPath.size() - 1; ++i) {
			if (!LineTest(lastPoint->GetCenterPos(), optimizedPath[i + 1]->GetCenterPos())) {
				straightPath.push_back(optimizedPath[i]);
				lastPoint = optimizedPath[i];
			}
		}
		straightPath.push_back(_end);
		return straightPath;
	}

	void APathFinder::MakeRoutine(SHPTR<ACell> _spStartCell, SHPTR<ACell> _spEndCell)
	{
		ACell* pCell = _spStartCell.get();
		m_CheckOpens[pCell->GetIndex()] = VISITED_VALUE;
		// Find Size만큼만 찾고 루트를 찾지 않는다(최적화를 위함)
		for (_int i = 0; i < FIND_SIZE; ++i)
		{
			_int Index = pCell->GetIndex();
			// 만약 방문했던 노드가 비어있지 않으면 제거
			if (false == m_VisitedListes.empty())
			{
				m_VisitedListes.pop_front();
			}

			for (auto& CheckCell : pCell->GetNeighborCells())
			{
				SHPTR<ACell> spCheckCell = CheckCell.lock();

				_int CheckIndex = spCheckCell->GetIndex();
				if (VISITED_VALUE == m_CheckOpens[CheckIndex])
					continue;

				m_SavePathList.push_front(spCheckCell);
				if (_spEndCell == spCheckCell)
				{
					return;
				}
				// 코스트를 구함 
				_float Cost = ComputeDistCost(_spStartCell->GetCenterPos(), _spEndCell->GetCenterPos(), 
					spCheckCell->GetCenterPos(), pCell->GetCenterPos());
				// CheckOpen 
				m_CheckOpens[spCheckCell->GetIndex()] = VISITED_VALUE;
				// 방문한 노드에 집어 넣는다. 
				m_VisitedListes.push_back(spCheckCell.get());
			}
			if (0 < m_VisitedListes.size())
			{
				VECTOR<_float>& DistanceWeights = m_DistanceWeights;
				m_VisitedListes.sort([&DistanceWeights]( ACell* _visited1, ACell* _visited2) {
					_int Index1 = _visited1->GetIndex();
					_int Index2 = _visited2->GetIndex();
					return DistanceWeights[Index1] < DistanceWeights[Index2];
					});

				pCell =  m_VisitedListes.front();
			}
			else
			{
				break;
			}
		}
	}

	void APathFinder::Release()
	{
		// value release
		MemoryInitialization(&m_CheckOpens[0], m_MaxCellCount);
		MemoryInitialization(&m_DistanceWeights[0], m_MaxCellCount);

		m_SavePathList.clear();
		m_VisitedListes.clear();
	}

	_float APathFinder::ComputeDistCost(const Vector3 _vStart, const Vector3 _vEnd, 
		const Vector3 _vVisited, const Vector3 _vCur)
	{
		// 시작지에서 방문지까지 거리 
		Vector3 vCostF = _vVisited - _vStart;
		// 방문지에서 목표까지 거리
		Vector3 vCostG = _vVisited - _vEnd;
		// 방문지에서 현재 있는 타일까지 거리
		Vector3 vCostT = _vVisited - _vCur;
		/* YPos */
		_float PosYF = vCostF.y; _float PosGF = vCostG.y; _float PosTF = vCostT.y;
		vCostF.y = 0; vCostG.y = 0; vCostT.y = 0;
		/* Weight */
		return Vector3::LengthSquared(vCostF) +
			Vector3::LengthSquared(vCostG) +
			Vector3::LengthSquared(vCostT) + PosYF + PosGF + PosTF;
	}

	_bool APathFinder::LineTest(Vector3 start, Vector3 end)
	{
		Vector3 direction = end - start;
		_float distance = static_cast<_float>(direction.Length());
		direction.Normalize();

		Vector3 currentPos = start;
		SHPTR<ACell> currentCell = m_spNavigation->FindCellWithoutUpdate(currentPos);

		if (!currentCell) {
			return false;
		}

		for (float t = 0; t < distance; t += 2.0f) {
			currentPos = start + direction * t;
			_int iNeighborIndex;

			if (!currentCell->IsIn(currentPos, iNeighborIndex)) {
				if (iNeighborIndex >= 0) {
					SHPTR<ACell> neighborCell = m_spNavigation->GetCell(iNeighborIndex);
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

	void APathFinder::Free()
	{
	}
}