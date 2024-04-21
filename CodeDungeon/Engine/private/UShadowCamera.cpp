#include "EngineDefine.h"
#include "UShadowCamera.h"

UShadowCamera::UShadowCamera(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UCamera(_spDevice, _wstrLayer, _eCloneType)
{
}

UShadowCamera::UShadowCamera(const UShadowCamera& _rhs) :
	UCamera(_rhs)
{
}

void UShadowCamera::Free()
{
}

HRESULT UShadowCamera::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UShadowCamera::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	SetCamID(SHADOWLIGHT_CAMERA_ID);
	return __super::NativeConstructClone(_vecDatas);
}

void UShadowCamera::TickActive(const _double& _dTimeDelta)
{
}

void UShadowCamera::LateTickActive(const _double& _dTimeDelta)
{
}
