#include "EngineDefine.h"
#include "UCharacter.h"
#include "UTransform.h"

UCharacter::UCharacter(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, 
	const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC)
{
}

UCharacter::UCharacter(const UCharacter& _rhs) : UPawn(_rhs)
{
}

void UCharacter::Free()
{
}

HRESULT UCharacter::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UCharacter::NativeConstructClone(const VOIDDATAS& _Datas)
{
	return __super::NativeConstructClone(_Datas);
}

_float UCharacter::OtherCharacterToDistance(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterToDistance(_spOtherActor->GetTransform());
}

_float UCharacter::OtherCharacterDirToLook(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterDirToLook(_spOtherActor->GetTransform());
}

_float UCharacter::OhterCharacterDirToRight(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OhterCharacterDirToRight(_spOtherActor->GetTransform());
}

_float UCharacter::OtherCharacterDirToLookConverter(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterDirToLookConverter(_spOtherActor->GetTransform());
}

_float3 UCharacter::OtherCharacterDirToLookVectorF3(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterDirToLookVectorF3(_spOtherActor->GetTransform());
}

_float UCharacter::OtherCharacterToDistance(CSHPTRREF<UTransform> _spOtherTransform) 
{
	SHPTR<UTransform> spTransform = GetTransform();
	assert(nullptr != spTransform && nullptr != _spOtherTransform);

	_float fDistance = 0.f;

	fDistance = DirectX::XMVectorGetX(DirectX::XMVector3Length(spTransform->GetPos()
		- _spOtherTransform->GetPos()));

	return fDistance;
}
_float UCharacter::OtherCharacterDirToLook(CSHPTRREF<UTransform> _spOtherTransform) 
{
	SHPTR<UTransform> spTransform = GetTransform();
	assert(nullptr != _spOtherTransform && nullptr != spTransform);

	_float3 v3Look;
	v3Look = (_spOtherTransform->GetPos() - spTransform->GetPos());
	return v3Look.Dot(spTransform->GetLook());
}
_float UCharacter::OhterCharacterDirToRight(CSHPTRREF<UTransform> _spOtherTransform) 
{
	SHPTR<UTransform> spTransform = GetTransform();
	assert(nullptr != _spOtherTransform && nullptr != spTransform);

	_float3 v3Look;
	v3Look = (_spOtherTransform->GetPos() - spTransform->GetPos());
	return v3Look.Dot(spTransform->GetRight());
}
_float UCharacter::OtherCharacterDirToLookConverter(CSHPTRREF<UTransform> _spOtherTransform) 
{
	_float fDot = DirectX::XMConvertToDegrees(acosf(OtherCharacterDirToLook(_spOtherTransform)));
	_float fValue = OhterCharacterDirToRight(_spOtherTransform);

	if (fValue < 0)
		fDot = -fDot;

	return fDot;
}

_float3 UCharacter::OtherCharacterDirToLookVectorF3(CSHPTRREF<UTransform> _spOtherTransform) 
{
	_float3 vDirection = _float3(0.f, 0.f, 0.f);
	assert(nullptr != _spOtherTransform);
	vDirection = _spOtherTransform->GetPos() - GetTransform()->GetPos();
	vDirection.Normalize();
	return vDirection;
}

void UCharacter::TickActive(const _double& _dTimeDelta)
{
}

void UCharacter::LateTickActive(const _double& _dTimeDelta)
{
}

HRESULT UCharacter::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void UCharacter::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
