#pragma once
#include "UEquipment.h"

BEGIN(Tool)

class TEquipModel final : public UEquipment {
public:
	TEquipModel(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	TEquipModel(const TEquipModel& _rhs);
	DESTRUCTOR(TEquipModel)
public:
	// UPawn을(를) 통해 상속됨
	CLONE_MACRO(TEquipModel, "TEquipModel::Clone To Failed")
	virtual void Free() override;
public:
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
public:

protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
protected:

};

END