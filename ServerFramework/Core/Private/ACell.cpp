#include "CoreDefines.h"
#include "ACell.h"

namespace Core {

	ACell::ACell() : m_vCenterPos{}, m_Index{0}, m_vPlane{}, 
		m_Points{}, m_Lines{}, m_Normals{}, m_NeighborIndexes{-1, -1, -1}
	{
	}

	_bool ACell::NativeConstruct(const ARRAY<Vector3, POINT_END>& _Points, const _int _iIndex)
	{
		::memcpy(&m_NeighborIndexes[0], &_Points[0], sizeof(Vector3) * POINT_END);
		m_Index = _iIndex;
		for (auto& iter : m_Points)
		{
			m_vCenterPos += iter;
		}
		m_vCenterPos /= 3;
		return true;
	}

	/*
	 Positon이 Cell 위에 있는지 판단하는 함수이다. 
	*/
	_bool ACell::IsIn(const Vector3& _vPos, REF_IN _int& _NeighborIndex)
	{
		for (_int i = 0; i < POINT_END; ++i){
			Vector3 vPoints = m_Points[i];
			Vector3 vDir = glm::normalize((_vPos - vPoints));

			Vector3 vNormal = m_Normals[i];
			_float D = glm::dot(vDir, vNormal);
			if (0 < D){
				_NeighborIndex = m_NeighborIndexes[i];
				return false;
			}
		}
		return true;
	}

	_bool ACell::UpdateNeighbor(SHPTR<ACell> _spCell, POINT _Point1, POINT _Point2, LINE _Line)
	{
		if (true == IsCompareCell(_spCell, _Point1, _Point2))
		{
			// 이웃을 등록한다. 
			_spCell->m_NeighborIndexes[_Line] = m_Index;
			_spCell->m_NeightborCells[_Line] = this;
			return true;
		}
		return false;
	}

	_bool ACell::IsCompareCell(SHPTR<ACell> _spCell, POINT _Point1, POINT _Point2)
	{
		if (m_Points[POINT_A] == _spCell->m_Points[_Point1])
		{
			RETURN_CHECK(m_Points[POINT_B] == _spCell->m_Points[_Point2], true);
			RETURN_CHECK(m_Points[POINT_C] == _spCell->m_Points[_Point2], true);
		}
		if (m_Points[POINT_B] == _spCell->m_Points[_Point1])
		{
			RETURN_CHECK(m_Points[POINT_A] == _spCell->m_Points[_Point2], true);
			RETURN_CHECK(m_Points[POINT_C] == _spCell->m_Points[_Point2], true);
		}
		if (m_Points[POINT_C] == _spCell->m_Points[_Point1])
		{
			RETURN_CHECK(m_Points[POINT_A] == _spCell->m_Points[_Point2], true);
			RETURN_CHECK(m_Points[POINT_B] == _spCell->m_Points[_Point2], true);
		}
		return true;
	}

	_float ACell::ComputeHeight(const Vector3& _vPosition)
	{
		Vector4 vPlane = m_vPlane;
		return PTH::PlaneDotCoord(vPlane, _vPosition) + _vPosition.y;
	}

	void ACell::ResortPoints()
	{
		while (true)
		{
			ARRAY<Vector3, POINT_END> Crosses;
			CalculateCrossResult(Crosses);
			
			if (Crosses[0].y < 0) {
				std::swap(m_Points[POINT_A], m_Points[POINT_B]);
				continue;
			}
			if (Crosses[1].y < 0) {
				std::swap(m_Points[POINT_B], m_Points[POINT_C]);
				continue;
			}
			if (Crosses[2].y < 0) {
				std::swap(m_Points[POINT_C], m_Points[POINT_A]);
				continue;
			}
			break;
		}
		MakeLineAndNormal();
	}

	void ACell::CalculateCrossResult(ARRAY<Vector3, POINT_END>& _Crosses)
	{
		// LINE 
		Vector3 L1 = m_Points[POINT_B] - m_Points[POINT_A];
		Vector3 L2 = m_Points[POINT_C] - m_Points[POINT_B];
		Vector3 L3 = m_Points[POINT_A] - m_Points[POINT_C];
		L1.y = 0; L2.y = 0; L3.y = 0;
		// CROSS
		_Crosses[POINT_A] = glm::cross(L1, L2);
		_Crosses[POINT_B] = glm::cross(L2, L3);
		_Crosses[POINT_C] = glm::cross(L3, L1);
	}

	void ACell::MakeLineAndNormal()
	{
		m_Lines[LINE_AB] = m_Points[POINT_B] - m_Points[POINT_A];
		m_Lines[LINE_BC] = m_Points[POINT_C] - m_Points[POINT_B];
		m_Lines[LINE_CA] = m_Points[POINT_A] - m_Points[POINT_C];

		m_Normals[LINE_AB] = Vector3(m_Lines[LINE_AB].z * -1.f, 0.f, m_Lines[LINE_AB].x);
		m_Normals[LINE_BC] = Vector3(m_Lines[LINE_BC].z * -1.f, 0.f, m_Lines[LINE_BC].x);
		m_Normals[LINE_CA] = Vector3(m_Lines[LINE_CA].z * -1.f, 0.f, m_Lines[LINE_CA].x);

		m_vPlane = PTH::PlaneFromPoints(m_Points[POINT_A], m_Points[POINT_B], m_Points[POINT_C]);
	}

	void ACell::Free()
	{
	}

}
