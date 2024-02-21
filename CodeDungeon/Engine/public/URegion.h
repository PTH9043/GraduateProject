#pragma once
#include "UResource.h"

BEGIN(Engine)
class UNavigation;
class UCell;
//class UCollider;
class URenderer;
class UDefaultCube;

class URegion : public UResource
{
public:
#ifdef _USE_DEBUGGING
	typedef struct tagCubeObjs
	{
		SHPTR<UCell> spCell;
		SHPTR<UDefaultCube> spCube1;
		SHPTR<UDefaultCube> spCube2;
		SHPTR<UDefaultCube> spCube3;

		void Create(SHPTRREF <UCell> _pCell);
		void Rebalance();
		void Render_Cubes(CSHPTRREF<URenderer> _pRenderer); 
	}CUBOBJS;
#endif
	URegion(CSHPTRREF<UDevice> _spDevice);
	URegion(const URegion& _rhs);
	DESTRUCTOR(URegion)

public:
	CLONE_MACRO(URegion, "Region::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;


protected:
	SHPTR<UNavigation>	m_spNavigation;
	_uint	m_iIndex = 0;
	LIST<SHPTR<URegion>>	m_NeighborRegion;
	
#ifdef _USE_DEBUGGING
	LIST<SHPTR<CUBOBJS>>	m_CubeObjList;
#endif
};

END