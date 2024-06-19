#include "CoreDefines.h"
#include "APathFinder.h"
#include "ANavigation.h"
#include "ACell.h"

namespace Core {

	APathFinder::APathFinder() : 
		m_MaxCellCount{0}, m_CheckOpens{}, m_DistanceWeights{}
		,m_VisitedListes{}, m_BestListes{}
	{
	}

	_bool APathFinder::NativeConstruct(const VECTOR<SHPTR<ACell>>& _Cells)
	{
		// Cells
		m_MaxCellCount = static_cast<_int>(_Cells.size());
		m_ParentsCellNodes.resize(m_MaxCellCount);
		// value 
		m_CheckOpens.resize(m_MaxCellCount);
		m_DistanceWeights.resize(m_MaxCellCount);
		return true;
	}

	void APathFinder::FindPath(SHPTR<ANavigation> _spNavigation, Vector3 _vStartPos, Vector3 _vEndPos)
	{
		RETURN_CHECK(nullptr == _spNavigation, ;);
		Release();
		SHPTR<ACell> spStartCell = _spNavigation->FindCell(_vStartPos);
		SHPTR<ACell> spEndCell = _spNavigation->FindCell(_vEndPos);
		RETURN_CHECK(nullptr == spStartCell || nullptr == spEndCell, ;);
		MakeRoutine(spStartCell, spEndCell);
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

				m_ParentsCellNodes[CheckIndex] = spCheckCell.get();
				if (_spEndCell == spCheckCell)
				{
					MakeBestRoutine(_spStartCell.get(), spCheckCell.get());
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
		MakeBestRoutine(_spStartCell.get(), pCell);
	}

	void APathFinder::MakeBestRoutine(ACell* _pStartCell, ACell* _pEndCell)
	{
		ACell* pCell = _pEndCell;
		m_BestListes.clear();
		while (true)
		{
			RETURN_CHECK(nullptr == pCell, ;);
			m_BestListes.push_back(pCell->GetCenterPos());
			_int index = pCell->GetIndex();
			pCell = m_ParentsCellNodes[index];
			RETURN_CHECK(_pStartCell == pCell, ;);
		}
	}

	void APathFinder::Release()
	{
		// value release
		MemoryInitialization(&m_CheckOpens[0], m_MaxCellCount);
		MemoryInitialization(&m_DistanceWeights[0], m_MaxCellCount);

		m_VisitedListes.clear();
		m_BestListes.clear();
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
		return glm::length2(vCostF) + glm::length2(vCostG) + glm::length2(vCostT) + PosYF + PosGF + PosTF;
	}

	void APathFinder::Free()
	{
	}
}