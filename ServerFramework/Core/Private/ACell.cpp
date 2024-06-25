#include "CoreDefines.h"
#include "ACell.h"

namespace Core {


	ACell::ACell() :
		m_arrPoints{},
		m_arrLines{},
		m_arrNormals{},
		m_arrNeighbors{ -1, -1, -1 },
		m_vCenterPos{},
		m_iIndex{ 0 },
		m_vPlane{},
		m_isJumpable{false}
	{
	}
	_bool ACell::NativeConstruct(const ARRAY<Vector3, POINT_END>& _Points, const _int _iIndex)
	{
		::memcpy(&m_arrNeighbors[0], &_Points[0], sizeof(Vector3) * POINT_END);
		m_iIndex = _iIndex;
		ResortPoints();

		for (auto& iter : m_arrPoints)
		{
			m_vCenterPos += iter;
		}
		m_vCenterPos /= 3;
		return true;
	}

	_bool ACell::NativeConstruct(const CELLDECS& _tCellDesc)
	{
		for (_uint i = 0; i < POINT_END; ++i)
		{
			m_arrPoints[i] = _tCellDesc.vPoints[i];
			m_arrNormals[i] = _tCellDesc.vNormal[i];
		}
		for (_uint i = 0; i < LINE_END; ++i)
		{
			m_arrLines[i] = _tCellDesc.vLine[i];
			m_arrNeighbors[i] = _tCellDesc.iNeighbor[i];
		}
		m_iIndex = _tCellDesc.iIndex;
		m_isJumpable = _tCellDesc.bisJumpable;

		ResortPoints();

		for (auto& iter : m_arrPoints)
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
			Vector3 vPoints = m_arrPoints[i];
			Vector3 vDir = glm::normalize((_vPos - vPoints));

			Vector3 vNormal = m_arrNormals[i];
			_float D = glm::dot(vDir, vNormal);
			if (0 < D){
				_NeighborIndex = m_arrNeighbors[i];
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
			_spCell->m_arrNeighbors[_Line] = m_iIndex;
			_spCell->m_arrNeighborCells[_Line] = ThisShared<ACell>();
			return true;
		}
		return false;
	}

	_bool ACell::IsCompareCell(SHPTR<ACell> _spCell, POINT _Point1, POINT _Point2)
	{
		if (m_arrPoints[POINT_A] == _spCell->m_arrPoints[_Point1])
		{
			RETURN_CHECK(m_arrPoints[POINT_B] == _spCell->m_arrPoints[_Point2], true);
			RETURN_CHECK(m_arrPoints[POINT_C] == _spCell->m_arrPoints[_Point2], true);
		}
		if (m_arrPoints[POINT_B] == _spCell->m_arrPoints[_Point1])
		{
			RETURN_CHECK(m_arrPoints[POINT_A] == _spCell->m_arrPoints[_Point2], true);
			RETURN_CHECK(m_arrPoints[POINT_C] == _spCell->m_arrPoints[_Point2], true);
		}
		if (m_arrPoints[POINT_C] == _spCell->m_arrPoints[_Point1])
		{
			RETURN_CHECK(m_arrPoints[POINT_A] == _spCell->m_arrPoints[_Point2], true);
			RETURN_CHECK(m_arrPoints[POINT_B] == _spCell->m_arrPoints[_Point2], true);
		}
		return true;
	}

	_float ACell::ComputeHeight(const Vector3& _vPosition)
	{
		Vector4 vPlane = m_vPlane;
		return PTH::PlaneDotCoord(vPlane, _vPosition) + _vPosition.y;
	}

	Vector3 ACell::GetClosestPointOnEdges(const Vector3& position) const
	{
		Vector3 closestPoint = position;
		_float minDistanceSquared = FLT_MAX;

		// 각 변에 대해 가장 가까운 점을 찾음
		for (size_t i = 0; i < LINE_END; ++i) {
			Vector3 pointOnEdge = ClosestPointOnLine(m_arrPoints[i], m_arrPoints[(i + 1) % LINE_END], position);
			_float distanceSquared =  glm::distance2(position, pointOnEdge);
			if (distanceSquared < minDistanceSquared) {
				minDistanceSquared = distanceSquared;
				closestPoint = pointOnEdge;
			}
		}
		return closestPoint;
	}

	Vector3 ACell::ClosestPointOnLine(const Vector3& lineStart, const Vector3& lineEnd, const Vector3& point) const
	{
		// 라인 시작점부터 끝점까지의 벡터
		Vector3 lineDirection = lineEnd - lineStart;

		// 라인 시작점부터 입력된 점까지의 벡터
		Vector3 pointToLineStart = point - lineStart;

		// 라인 방향과 입력된 점까지의 벡터 사이의 내적 값
		float t = glm::dot(pointToLineStart, lineDirection) / glm::dot(lineDirection, lineDirection);

		// t 값을 사용하여 입력된 점에서 가장 가까운 점을 계산
		Vector3 closestPoint = lineStart + lineDirection * t;
		if (t > 1.0f) {
			closestPoint = lineEnd;
		}

		else if (t < 0.0f) {
			closestPoint = lineStart;
		}
		return closestPoint;
	}

	_float ACell::GetHeightAtXZ(const _float& x, const _float& z) const
	{
		// Points of the triangle
		Vector3 P1 = m_arrPoints[POINT_A];
		Vector3 P2 = m_arrPoints[POINT_B];
		Vector3 P3 = m_arrPoints[POINT_C];

		// Calculate the plane equation coefficients
		_float A = (P2.y - P1.y) * (P3.z - P1.z) - (P2.z - P1.z) * (P3.y - P1.y);
		_float B = (P2.z - P1.z) * (P3.x - P1.x) - (P2.x - P1.x) * (P3.z - P1.z);
		_float C = (P2.x - P1.x) * (P3.y - P1.y) - (P2.y - P1.y) * (P3.x - P1.x);
		_float D = -(A * P1.x + B * P1.y + C * P1.z);

		_float y = -(A * x + C * z + D) / B;

		return y;
	}

	SHPTR<ACell> ACell::FindNeighborCell(const _int _iIndex)
	{
		RETURN_CHECK(_iIndex >= POINT_END, nullptr);
		return m_arrNeighborCells[_iIndex].lock();
	}

	void ACell::ResortPoints()
	{
		while (true)
		{
			ARRAY<Vector3, POINT_END> Crosses;
			CalculateCrossResult(Crosses);
			
			if (Crosses[0].y < 0) {
				std::swap(m_arrPoints[POINT_A], m_arrPoints[POINT_B]);
				continue;
			}
			if (Crosses[1].y < 0) {
				std::swap(m_arrPoints[POINT_B], m_arrPoints[POINT_C]);
				continue;
			}
			if (Crosses[2].y < 0) {
				std::swap(m_arrPoints[POINT_C], m_arrPoints[POINT_A]);
				continue;
			}
			break;
		}
		MakeLineAndNormal();
	}

	void ACell::CalculateCrossResult(ARRAY<Vector3, POINT_END>& _Crosses)
	{
		// LINE 
		Vector3 L1 = m_arrPoints[POINT_B] - m_arrPoints[POINT_A];
		Vector3 L2 = m_arrPoints[POINT_C] - m_arrPoints[POINT_B];
		Vector3 L3 = m_arrPoints[POINT_A] - m_arrPoints[POINT_C];
		L1.y = 0; L2.y = 0; L3.y = 0;
		// CROSS
		_Crosses[POINT_A] = glm::cross(L1, L2);
		_Crosses[POINT_B] = glm::cross(L2, L3);
		_Crosses[POINT_C] = glm::cross(L3, L1);
	}

	void ACell::MakeLineAndNormal()
	{
		m_arrLines[LINE_AB] = m_arrPoints[POINT_B] - m_arrPoints[POINT_A];
		m_arrLines[LINE_BC] = m_arrPoints[POINT_C] - m_arrPoints[POINT_B];
		m_arrLines[LINE_CA] = m_arrPoints[POINT_A] - m_arrPoints[POINT_C];

		m_arrNormals[LINE_AB] = Vector3(m_arrLines[LINE_AB].z * -1.f, 0.f, m_arrLines[LINE_AB].x);
		m_arrNormals[LINE_BC] = Vector3(m_arrLines[LINE_BC].z * -1.f, 0.f, m_arrLines[LINE_BC].x);
		m_arrNormals[LINE_CA] = Vector3(m_arrLines[LINE_CA].z * -1.f, 0.f, m_arrLines[LINE_CA].x);

		m_vPlane = PTH::PlaneFromPoints(m_arrPoints[POINT_A], m_arrPoints[POINT_B], m_arrPoints[POINT_C]);
	}

	void ACell::Free()
	{
	}

}
