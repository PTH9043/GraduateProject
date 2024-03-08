#include "ClientDefines.h"
#include "UPlayer.h"

UPlayer::UPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UCharacter(_spDevice, _wstrLayer, _eCloneType)
{
}

UPlayer::UPlayer(const UPlayer& _rhs) : 
	UCharacter(_rhs)
{
}

void UPlayer::Free()
{
}

HRESULT UPlayer::NativeConstruct()
{
	return E_NOTIMPL;
}

HRESULT UPlayer::NativeConstructClone(const VOIDDATAS& _Datas)
{
	return E_NOTIMPL;
}

void UPlayer::TickActive(const _double& _dTimeDelta)
{
}

void UPlayer::LateTickActive(const _double& _dTimeDelta)
{
}

HRESULT UPlayer::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return E_NOTIMPL;
}

void UPlayer::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
