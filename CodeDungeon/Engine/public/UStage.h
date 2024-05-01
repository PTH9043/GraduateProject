#pragma once

#include "UComponent.h"
#include <map>

BEGIN(Engine)
class URegion;
class UCell;
class UCollider;

using REGIONLIST = VECTOR<SHPTR<URegion>>;
using FILEPAIR = std::pair<_wstring, SHPTR<FILEDATA>>;

class UStage : public UComponent
{
public:

	UStage(CSHPTRREF<UDevice> _spDevice);
	UStage(const UStage& _rhs);
	DESTRUCTOR(UStage)

public:
	CLONE_MACRO(UStage, "Stage::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	
	virtual HRESULT AddRender(const _uint& _iIndex);
	void AddRenderAll();

	CSHPTRREF<REGIONLIST> GetRegionList() { return m_spRegionList; }
	SHPTR<URegion> GetRegion(const _uint& _iIndex);

	HRESULT AddCell(const _uint& _iCellIndex, SHPTR<UCell>& _pCell);
	HRESULT ModifyCells(const _uint& _iCellIndex);
	HRESULT ShowCells(const _uint& _iCellIndex);
	HRESULT ClearCell(const _uint& _iCellIndex);
	HRESULT SetColor(const _uint& _iCellIndex);
	HRESULT SetRegionName(const _uint& _iCellIndex);
	HRESULT FlushDeleteCells();

	_bool Load();
	_bool Save(const _wstring& _wstrPath);

	HRESULT CreateRegion();
	_int SelectRegion();
	void Control_Collider(const _uint& _iIndex);
	HRESULT Delete_Region(_uint& _iIndex);

	// Is Collision
	_bool Is_Collision(SHPTR<UCollider>& _pCollider, SHPTR<URegion>* _ppOut = nullptr);
	// Is Picking
	_bool Is_Picking(const _uint& _iIndex = 0);
	// ModelPicking
	_bool Is_ModelPicking(const _uint& _iIndex = 0);

	// Update Region
	void UpdateRegion();
	// Add Arround Region
	void AddArroundRegion();

protected:
	SHPTR<REGIONLIST> m_spRegionList;
	_bool	m_bInitRegion;

};

END