#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H

#include "UBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-14, Writer: 박태현
@ Explain
-  해당 길 위에 있지 않는지 판단하고, 내가 원하는 위치까지를 찾아줄 클래스
 Index 같은 경우 공용 값이 아니므로 datarace가 일어날 일이 존재하지 않는다. 그래서 atomic을 쓰지 않는다.
*/
class UCell;
using CELLCONTAINER = VECTOR<SHPTR<UCell>>;

class CACHE_ALGIN_CORE_DLL UNavigation final : public UBase {
public:
	UNavigation();
	NO_COPY(UNavigation)
	DESTRUCTOR(UNavigation)
public:
	_bool NativeConstruct(const _string& _Paths);
	// 객체의 Y값이 해당 Cell 바로 위로 만드는 함수 
	const _float ComputeHeight(const _float3 _vPosition, REF_IN  _int& _Index);
	// 네비게이션의 범위를 벗어나는지 확인하는 함수
	_bool IsCheckOverNavigationArea(const _float3 _vPosition, REF_IN _int& _Index);
	// Cell을 찾아 리턴하는 함수
	SHPTR<UCell>	FindCell(const _float3 _vPosition, REF_IN _int& _Index);
	SHPTR<UCell> FindCell(const _float3 _vPosition);
private:
	void ReadyNeighbor();
private:
	virtual void Free() override;
private:
	static constexpr	_int		FINDCELL_CNT{ 10 };
	// MaxCellCount 
	_int										m_MaxCellCount;
	CELLCONTAINER				m_CellContainer;
};

END

#endif //_SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H