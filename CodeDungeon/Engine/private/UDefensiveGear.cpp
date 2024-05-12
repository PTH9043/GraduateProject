#include "EngineDefine.h"
#include "UDefensiveGear.h"

BEGIN(Engine)

UDefensiveGear::UDefensiveGear(CSHPTRREF<UDevice> _spDevice, 
	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UEquipment(_spDevice, _wstrLayer, _eCloneType, EQUIP_DEFENSIVEGEAR)
{
}

UDefensiveGear::UDefensiveGear(const UDefensiveGear& _rhs) : 
	UEquipment(_rhs)
{
}

void UDefensiveGear::Free()
{
}

HRESULT UDefensiveGear::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UDefensiveGear::NativeConstructClone(const VOIDDATAS& _Datas)
{
	return __super::NativeConstructClone(_Datas);
}

void UDefensiveGear::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}

void UDefensiveGear::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT UDefensiveGear::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT UDefensiveGear::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return E_NOTIMPL;
}

void UDefensiveGear::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}


END
