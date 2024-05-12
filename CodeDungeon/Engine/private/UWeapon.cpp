#include "EngineDefine.h"
#include "UWeapon.h"

BEGIN(Engine)

UWeapon::UWeapon(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, 
	const CLONETYPE& _eCloneType) : 
	UEquipment(_spDevice, _wstrLayer, _eCloneType, EQUIP_WEAPON),
	m_eWeaponType{WEAPONTYPE::WEAPON_END }
{
}

UWeapon::UWeapon(const UWeapon& _rhs) : 
	UEquipment(_rhs), m_eWeaponType{_rhs.m_eWeaponType}
{
}

void UWeapon::Free()
{
}

HRESULT UWeapon::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UWeapon::NativeConstructClone(const VOIDDATAS& _Datas)
{
	if (FAILED(__super::NativeConstructClone(_Datas)))
		return E_FAIL;

	return S_OK;
}

void UWeapon::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}

void UWeapon::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT UWeapon::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT UWeapon::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return E_NOTIMPL;
}

void UWeapon::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

END