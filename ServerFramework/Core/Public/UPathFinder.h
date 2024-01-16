#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H

#include "UBase.h"

BEGIN(Core)
class UCell;
class UNavigation;

class CACHE_ALGIN_CORE_DLL UPathFinder final : public UBase {
public:
	UPathFinder();
	NO_COPY(UPathFinder)
	DESTRUCTOR(UPathFinder)
public:
	_bool NativeConstruct(const VECTOR<SHPTR<UCell>>& _Cells);
	// Find Path
	void FindPath(SHPTR<UNavigation> _spNavigation,
		_float3 _vStartPos, _float3 _vEndPos);
public: /* Get Set */
	LIST<UCell*> GetBestList() const { return m_BestListes; }
private:
	void MakeRoutine(SHPTR<UCell> _spStartCell, SHPTR<UCell> _spEndCell);
	void MakeBestRoutine(UCell* _pStartCell, UCell* _pEndCell);
	void Release();

	_float ComputeDistCost(const _float3 _vStart, const _float3 _vEnd,
		const _float3 _vVisited, const _float3 _vCur);
private:
	virtual void Free() override;
private:
	static constexpr  _int		FIND_SIZE{ 500 };
	static constexpr _int		VISITED_VALUE{ 1 };
	_uint									m_MaxCellCount;
	// Nodes 
	VECTOR<UCell*>				m_ParentsCellNodes;
	// Open Close
	VECTOR<_int>					m_CheckOpens;
	// Weight
	VECTOR<_float>				m_DistanceWeights;
	// List
	LIST<UCell*>						m_VisitedListes;
	LIST<UCell*>						m_BestListes;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H