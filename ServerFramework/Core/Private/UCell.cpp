#include "CoreDefines.h"
#include "UCell.h"

namespace Core {

	UCell::UCell() : m_vCenterPos{}, m_Index{0}, m_vPlane{}, 
		m_Points{}, m_Lines{}, m_Normals{}, m_Neighbors{-1, -1, -1}
	{
	}

	_bool UCell::NativeConstruct(const ARRAY<_float3, POINT_END>& _Points, const _int _iIndex)
	{
		::memcpy(&m_Neighbors[0], &_Points[0], sizeof(_float3) * POINT_END);
		m_Index = _iIndex;
		for (auto& iter : m_Points)
		{
			m_vCenterPos += iter;
		}
		m_vCenterPos /= 3;
		return true;
	}

	void UCell::ResortPoints()
	{
		while (true)
		{
			ARRAY<_float3, POINT_END> Crosses;
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

	void UCell::CalculateCrossResult(ARRAY<_float3, POINT_END>& _Crosses)
	{
		// LINE 
		_float3 L1 = m_Points[POINT_B] - m_Points[POINT_A];
		_float3 L2 = m_Points[POINT_C] - m_Points[POINT_B];
		_float3 L3 = m_Points[POINT_A] - m_Points[POINT_C];
		L1.y = 0; L2.y = 0; L3.y = 0;
		// CROSS
		_Crosses[POINT_A] = glm::cross(L1, L2);
		_Crosses[POINT_B] = glm::cross(L2, L3);
		_Crosses[POINT_C] = glm::cross(L3, L1);
	}

	void UCell::MakeLineAndNormal()
	{
		m_Lines[LINE_AB] = m_Points[POINT_B] - m_Points[POINT_A];
		m_Lines[LINE_BC] = m_Points[POINT_C] - m_Points[POINT_B];
		m_Lines[LINE_CA] = m_Points[POINT_A] - m_Points[POINT_C];

		m_Normals[LINE_AB] = _float3(m_Lines[LINE_AB].z * -1.f, 0.f, m_Lines[LINE_AB].x);
		m_Normals[LINE_BC] = _float3(m_Lines[LINE_BC].z * -1.f, 0.f, m_Lines[LINE_BC].x);
		m_Normals[LINE_CA] = _float3(m_Lines[LINE_CA].z * -1.f, 0.f, m_Lines[LINE_CA].x);

		m_vPlane = PTH::PlaneFromPoints(m_Points[POINT_A], m_Points[POINT_B], m_Points[POINT_C]);
	}

	void UCell::Free()
	{
	}

}
