#include "CoreDefines.h"
#include "ANavigation.h"
#include <fstream>
#include "ACell.h"

namespace Core {
	ANavigation::ANavigation() : m_MaxCellCount{0}
	{
	}

	_bool ANavigation::NativeConstruct(const _string& _Paths)
	{
		std::ofstream LOAD{ _Paths, std::ios_base::binary };
		RETURN_CHECK(false == LOAD.is_open(), false);

		// Neightbor setting
		ReadyNeighbor();
		return true;
	}

	const _float ANavigation::ComputeHeight(const Vector3 _vPosition, REF_IN  _int& _Index)
	{
		RETURN_CHECK(_Index >= m_MaxCellCount, _vPosition.y);
		SHPTR<ACell> spCell = m_CellContainer[_Index];
		if (nullptr != spCell)
		{
			return spCell->ComputeHeight(_vPosition);
		}
		else
		{
			for (auto iter : m_CellContainer)
			{
				if (true == iter->IsIn(_vPosition, REF_OUT _Index))
				{
					spCell = iter;
					break;
				}
			}
			RETURN_CHECK(nullptr == spCell, _vPosition.y);
			return spCell->ComputeHeight(_vPosition);
		}
		return _vPosition.y;
	}

	_bool ANavigation::IsCheckOverNavigationArea(const Vector3 _vPosition, REF_IN  _int& _Index)
	{
		_int Index = _Index;
		if (-1 == Index || Index >= m_MaxCellCount)
		{
			return false;
		}

		_int NeighborIndex{ -1 };
		if (true == m_CellContainer[Index]->IsIn(_vPosition, REF_OUT NeighborIndex))
		{
			_Index = m_CellContainer[Index]->GetIndex();
			return true;
		}
		else
		{
			if (0 <= NeighborIndex)
			{
				_int Value{ 0 };
				while (Value++ != FINDCELL_CNT)
				{
					RETURN_CHECK(-1 == NeighborIndex, false);
					if (true == m_CellContainer[NeighborIndex]->IsIn(_vPosition, REF_OUT NeighborIndex))
					{
						_Index = m_CellContainer[NeighborIndex]->GetIndex();
						return true;
					}
				}
				// 만약 길이 없으면 Cell의 위치를 찾는다.
				SHPTR<ACell> spCell = FindCell(_vPosition, _Index);
				RETURN_CHECK(nullptr == spCell, false);
				return true;
			}
		}
		return false;
	}

	SHPTR<ACell> ANavigation::FindCell(const Vector3 _vPosition, REF_IN  _int& _Index)
	{
		_int NeighborIndex{ 0 };
		for (auto iter : m_CellContainer)
		{
			if (true == iter->IsIn(_vPosition, NeighborIndex))
			{
				_Index = iter->GetIndex();
				return iter;
			}
		}
		return nullptr;
	}

	SHPTR<ACell> ANavigation::FindCell(const Vector3 _vPosition)
	{
		_int NeighborIndex{ 0 };
		for (auto iter : m_CellContainer)
		{
			if (true == iter->IsIn(_vPosition, NeighborIndex))
			{
				return iter;
			}
		}
		return nullptr;
	}

	void ANavigation::ReadyNeighbor()
	{
		for (auto SourCell : m_CellContainer)
		{
			for (auto DestCell : m_CellContainer)
			{
				if (SourCell == DestCell)
					continue;

				if (true == DestCell->UpdateNeighbor(SourCell, ACell::POINT_A, ACell::POINT_B, ACell::LINE_AB))
					continue;
				if (true == DestCell->UpdateNeighbor(SourCell, ACell::POINT_B, ACell::POINT_C, ACell::LINE_BC))
					continue;
				if (true == DestCell->UpdateNeighbor(SourCell, ACell::POINT_C, ACell::POINT_A, ACell::LINE_CA))
					continue;
			}
		}
	}

	void ANavigation::Free()
	{
	}
}