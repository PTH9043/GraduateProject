#pragma once
#include "UComponent.h"

BEGIN(Engine)
class UNavigation;
class UCell;
class UCollider;
class URenderer;
class UDefaultDebugging;

class URegion : public UComponent
{
public:
#ifdef _USE_DEBUGGING
	typedef struct tagCubeObjs
	{
		SHPTR<UCell> spCell;
		SHPTR<UDefaultDebugging> spCube1;
		SHPTR<UDefaultDebugging> spCube2;
		SHPTR<UDefaultDebugging> spCube3;

		void Create(SHPTRREF <UCell> _pCell);
		void Rebalance();
		void AddCubesRenderGroup();
	}CUBOBJS;
#endif
	HRESULT AddRegionRenderGroup();
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
	const _uint& Get_Index() const { return m_iIndex; }
	const _wstring& Get_Name() const { return m_wsRegionName; }
	void Set_Index(const _uint& _iIndex) { this->m_iIndex = _iIndex; }

	HRESULT AddCell(SHPTR<UCell>& _pCell);
	HRESULT ModifyCells();
	HRESULT ShowCells();
	HRESULT ClearCell();
	HRESULT SetColor();
	HRESULT SetName();
	void SetName(const _wstring& _regionName)
	{
		m_wsRegionName = _regionName;
	}
	HRESULT DeleteLatestCell();
	HRESULT DeleteCell(const _uint& _iIndex);
	void FlushDeleteCells();

	_bool Load(const _wstring& _wstrPath);
	_bool Save(const _wstring& _wstrPath);

	// Is Collision
	_bool Is_Collision(SHPTR<UCollider>& _pCollider);
	// Control Collider
	void Control_Collider();

	void ClearNeightborRegion() { m_NeighborRegion.clear(); }
	// Add Neighbor Region
	void Add_NeighborRegion(CSHPTRREF<URegion> _pRegion);

	const _bool& IsDeletion() const { return m_bDeletionEnabled; }
private:
	_float3				m_f3Color;
protected:
	SHPTR<UNavigation>	m_spNavigation;
	_uint	m_iIndex = 0;
	LIST<WKPTR<URegion>>	m_NeighborRegion;

	SET<SHPTR<UCell>> m_DeleteCellsList;
	_bool m_bDeletionEnabled;

	_wstring m_wsRegionName;
	_bool m_bEditName;

#ifdef _USE_DEBUGGING
	LIST<CUBOBJS>	m_CubeObjList;
	SET<SHPTR<UDefaultDebugging>> m_DeleteCubesList;
#endif
};

END