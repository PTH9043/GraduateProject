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
	enum POINT : _llong { POINT_A, POINT_B, POINT_C, POINT_END};
	enum LINE : _llong { LINE_AB, LINE_BC, LINE_CA, LINE_END };
public:
	UCell();
	NO_COPY(UCell)
	DESTRUCTOR(UCell)
public:
	_bool NativeConstruct(const ARRAY<_float3, POINT_END>& _Points, const _int _iIndex);


private:
	void ResortPoints();
	void CalculateCrossResult(ARRAY<_float3, POINT_END>& _Crosses);
	void MakeLineAndNormal();
private:
	virtual void Free() override;
private:
	_float3											m_vCenterPos;
	_int													m_Index;
	_float4											m_vPlane;
	// Points
	ARRAY<_float3, POINT_END>	m_Points;
	ARRAY<_float3, LINE_END>		m_Lines;
	ARRAY<_float3, LINE_END>		m_Normals;
	ARRAY<_int, LINE_END>				m_Neighbors;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UCELL_H