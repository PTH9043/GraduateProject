#include "CoreDefines.h"
#include "UPathFinder.h"
#include "UNavigation.h"
#include "UCell.h"

namespace Core {

	UPathFinder::UPathFinder() : 
		m_MaxCellCount{0}, m_CheckOpens{}, m_CheckCloses{}, m_DistanceWeights{}
		,m_VisitedListes{}, m_BestListes{}
	{
	}

	_bool UPathFinder::NativeConstruct(const VECTOR<SHPTR<UCell>>& _Cells)
	{
		// Cells
		m_MaxCellCount = static_cast<_int>(_Cells.size());
		m_ParentsCellNodes.resize(m_MaxCellCount);
		// value 
		for (_int i = 0; i < TLS::MAX_THREAD; ++i)
		{
			m_CheckCloses.resize(m_MaxCellCount);
			m_CheckOpens.resize(m_MaxCellCount);
			m_DistanceWeights.resize(m_MaxCellCount);
		}
		return true;
	}

	void UPathFinder::FindPath(SHPTR<UNavigation> _spNavigation, _float3 _vStartPos, _float3 _vEndPos)
	{
		RETURN_CHECK(nullptr == _spNavigation, ;);
		Release();
		SHPTR<UCell> spStartCell = _spNavigation->FindCell(_vStartPos);
		SHPTR<UCell> spEndCell = _spNavigation->FindCell(_vEndPos);
		RETURN_CHECK(nullptr == spStartCell || nullptr == spEndCell, ;);
		MakeRoutine(spStartCell, spEndCell);
	}

	void UPathFinder::MakeRoutine(SHPTR<UCell> _spStartCell, SHPTR<UCell> _spEndCell)
	{
		SHPTR<UCell> spTile = _spStartCell;
		// Find Size만큼만 찾고 루트를 찾지 않는다(최적화를 위함)
		for (_int i = 0; i < FIND_SIZE; ++i)
		{
			_int Index = spTile->GetIndex();
			// 만약 방문했던 노드가 비어있지 않으면 제거
			if (false == m_VisitedListes.empty())
			{
				m_VisitedListes.pop_front();
			}
			m_CheckOpens[Index] = VISITED_VALUE;

			for (auto& CheckTile : spTile->GetNeighborCelles())
			{
				_int CheckIndex = CheckTile->GetIndex();
				if (VISITED_VALUE == m_CheckOpens[CheckIndex])
					continue;

				m_ParentsCellNodes[CheckIndex] = CheckTile;
				if (_spEndCell == CheckTile)
				{

					return;
				}
				/* CheckCenterPos  */
				_float3 vDir = CheckTile->GetCenterPos() - _spStartCell->GetCenterPos();
				// Y는 따로
				_float YPos = vDir.y;
				vDir.y = 0;
				// 코스트를 구함 
				_float Cost = ComputeDistCost(_spStartCell->GetCenterPos(), _spEndCell->GetCenterPos(), 
					CheckTile->GetCenterPos(), spTile->GetCenterPos()) + glm::length2(vDir) + YPos;
				// CheckOpen 
				m_CheckOpens[CheckTile->GetIndex()] = true;
				// 방문한 노드에 집어 넣는다. 
				m_VisitedListes.push_back(CheckTile->GetIndex());
			}
			if (0 < m_VisitedListes.size())
			{
				VECTOR<_float>& DistanceWeights = m_DistanceWeights;
				m_VisitedListes.sort([&DistanceWeights](_int _Visited1, _int _Visited2) {
					return DistanceWeights[_Visited1] < DistanceWeights[_Visited2];
					});
			}
			else
			{
				break;
			}
		}
	}

	void UPathFinder::MakeBestRoutine(UCell* _pStartCell, UCell* _pEndCell)
	{
		UCell* pCell = _pEndCell;
		while (true)
		{
			RETURN_CHECK(nullptr == pCell, ;);
			m_BestListes.push_back(pCell);
			_int index = pCell->GetIndex();
			pCell = m_ParentsCellNodes[index];
			RETURN_CHECK(_pStartCell == pCell, ;);
		}
	}

	void UPathFinder::Release()
	{
		// value release
		MemoryInitialization(&m_CheckCloses[0], m_MaxCellCount);
		MemoryInitialization(&m_CheckOpens[0], m_MaxCellCount);
		MemoryInitialization(&m_DistanceWeights[0], m_MaxCellCount);

		m_VisitedListes.clear();
		m_BestListes.clear();
	}

	_float UPathFinder::ComputeDistCost(const _float3 _vStart, const _float3 _vEnd, 
		const _float3 _vVisited, const _float3 _vCur)
	{
		// 시작지에서 방문지까지 거리 
		_float3 vCostF = _vVisited - _vStart;
		// 방문지에서 목표까지 거리
		_float3 vCostG = _vVisited - _vEnd;
		// 방문지에서 현재 있는 타일까지 거리
		_float3 vCostT = _vVisited - _vCur;
		/* YPos */
		_float PosYF = vCostF.y; _float PosGF = vCostG.y; _float PosTF = vCostT.y;
		vCostF.y = 0; vCostG.y = 0; vCostT.y = 0;
		/* Weight */
		return glm::length2(vCostF) + glm::length2(vCostG) + glm::length2(vCostT) + PosYF + PosGF + PosTF;
	}

	void UPathFinder::Free()
	{
	}
}