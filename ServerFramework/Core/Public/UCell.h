#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCELL_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCELL_H

#include "UBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-14, Writer: 박태현
@ Explain
- Navigation으로 지형 충돌을 수행할 예정인데 그것을 위한 클래스로 3개의 점을 이루어서
삼각형을 이루고 그 위에서 플레이어가 벗어나는지 벗어나지 않는지 확인하는 클래스이다.
*/
class CACHE_ALGIN_CORE_DLL  UCell final : public UBase {
public:
	enum POINT : _int { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE : _int { LINE_AB, LINE_BC, LINE_CA, LINE_END };
public:
	UCell();
	NO_COPY(UCell)
		DESTRUCTOR(UCell)
public:
	_bool NativeConstruct(const ARRAY<_float3, POINT_END>& _Points, const _int _iIndex);
	_bool IsIn(const _float3& _vPos, REF_IN _int& _NeighborIndex);
	_bool UpdateNeighbor(SHPTR<UCell> _spCell, POINT _Point1, POINT _Point2, LINE _Line);
	_bool IsCompareCell(SHPTR<UCell> _spCell, POINT _Point1, POINT _Point2);
	_float ComputeHeight(const _float3& _vPosition);
public: /* Get Set */
	_float3 GetCenterPos() const { return m_vCenterPos; }
	_int GetIndex() const { return m_Index; }
	_float3 GetPoint(POINT _point) const { return m_Points[_point]; }
	// Index Return
	const ARRAY<_int, LINE_END>& GetNeightborIndexes() const { return m_NeighborIndexes; }
private:
	void ResortPoints();
	void CalculateCrossResult(ARRAY<_float3, POINT_END>& _Crosses);
	void MakeLineAndNormal();
private:
	virtual void Free() override;
private:
	_float3												m_vCenterPos;
	_int														m_Index;
	_float4												m_vPlane;
	// Points
	ARRAY<_float3, POINT_END>		m_Points;
	ARRAY<_float3, LINE_END>			m_Lines;
	ARRAY<_float3, LINE_END>			m_Normals;
	ARRAY<_int, LINE_END>					m_NeighborIndexes;
};
END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UCELL_H