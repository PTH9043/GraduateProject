#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H

#include "ACoreBase.h"

BEGIN(Core)
class ACell;
class ANavigation;

/*
@ Date: 2023-01-15, Writer: 박태현
@ Explain
-  AStar와 Navigation Mesh를 이용해서 길을 찾기 위한 클래스, 해당 클래스는 쓰레드 개수만큼 만들 예정이다. 
Work Thread만큼 만들지, Job스레드만큼 만들지는 아직 미정
*/
class CORE_DLL APathFinder final : public ACoreBase {
public:
	APathFinder();
	NO_COPY(APathFinder)
	DESTRUCTOR(APathFinder)
public:
	_bool NativeConstruct(SHPTR<ANavigation> _spNavigation, const VECTOR<SHPTR<ACell>>& _Cells);
	// Find Path
	SHPTR<ACell> FindPath(Vector3 _vStartPos, Vector3 _vEndPos);
private:
	void MakeRoutine(SHPTR<ACell> _spStartCell, SHPTR<ACell> _spEndCell);
	void Release();

	_float ComputeDistCost(const Vector3 _vStart, const Vector3 _vEnd,
		const Vector3 _vVisited, const Vector3 _vCur);
	_bool LineTest(Vector3 start, Vector3 end);
private:
	virtual void Free() override;
private:
	static constexpr  _int		FIND_SIZE{ 500 };
	static constexpr _int		VISITED_VALUE{ 1 };
	_uint									m_MaxCellCount;
	// Open Close
	VECTOR<_int>					m_CheckOpens;
	// Weight
	VECTOR<_float>				m_DistanceWeights;
	// List
	LIST<ACell*>						m_VisitedListes;
	LIST<SHPTR<ACell>>		m_SavePathList;

	SHPTR<ANavigation>		m_spNavigation;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H