#pragma once
#include "UComponent.h"

BEGIN(Engine)
class UNavigation;
class UCell;
//class UCollider;
class URenderer;
class UDefaultCube;

class URegion : public UComponent
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
	}CUBOBJS;
#endif
	struct REGIONDESC
	{
		_uint	m_iIndex;

		REGIONDESC() : m_iIndex{} {}
		REGIONDESC(const _uint& _iIndex) : m_iIndex{ _iIndex }{}
	};

	URegion(CSHPTRREF<UDevice> _spDevice);
	URegion(const URegion& _rhs);
	DESTRUCTOR(URegion)

public:
	CLONE_MACRO(URegion, "Region::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

public:
	SHPTR<UNavigation> GetNavigation() { return m_spNavigation; }
	const _uint& Get_Index() const { return this->m_iIndex; }
	void Set_Index(const _uint& _iIndex) { this->m_iIndex = _iIndex; }

	HRESULT AddCell(SHPTR<UCell>& _pCell);
	HRESULT ModifyCells();
	HRESULT ShowCells();
	HRESULT ClearCell();

	_bool Load(const _wstring& _wstrPath);
	_bool Save(const _wstring& _wstrPath);

	// Is Collision
	_bool Is_Collision(SHPTR<UCollider>& _pCollider);
	// Control Collider
	void Control_Collider();

	void ClearNeightborRegion() { m_NeighborRegion.clear(); }
	// Add Neighbor Region
	void Add_NeighborRegion(SHPTR<URegion>& _pRegion);



protected:
	SHPTR<UNavigation>	m_spNavigation;
	_uint	m_iIndex = 0;
	LIST<SHPTR<URegion>>	m_NeighborRegion;

#ifdef _USE_DEBUGGING
	LIST<CUBOBJS>	m_CubeObjList;
#endif
};

END