#include "EngineDefines.h"
#include "UMirrorCamera.h"

UMirrorCamera::UMirrorCamera(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UCamera(_spDevice, _wstrLayer, _eCloneType)
{
}

UMirrorCamera::UMirrorCamera(const UMirrorCamera& _rhs)
	:UCamera(_rhs)
{
}

void UMirrorCamera::Free()
{
}

HRESULT UMirrorCamera::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UMirrorCamera::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	return __super::NativeConstructClone(_vecDatas);
}

void UMirrorCamera::TickActive(const _double& _dTimeDelta)
{
}

void UMirrorCamera::LateTickActive(const _double& _dTimeDelta)
{
}
