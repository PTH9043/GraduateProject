#include "ToolDefines.h"
#include "TEquipModel.h"
#include "UEquipment.h"

TEquipModel::TEquipModel(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, 
	const CLONETYPE& _eCloneType) : 
	UEquipment(_spDevice, _wstrLayer, _eCloneType)
{
}

TEquipModel::TEquipModel(const TEquipModel& _rhs) :
	UEquipment(_rhs)
{
}

void TEquipModel::Free()
{
}

HRESULT TEquipModel::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TEquipModel::NativeConstructClone(const VOIDDATAS& _Datas)
{
	return __super::NativeConstructClone(_Datas);
}

void TEquipModel::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}

void TEquipModel::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT TEquipModel::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	if (FAILED(__super::RenderActive(_spCommand, _spTableDescriptor)))
		return E_FAIL;

	return S_OK;
}

HRESULT TEquipModel::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void TEquipModel::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
