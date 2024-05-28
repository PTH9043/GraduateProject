#include "EngineDefine.h"
#include "UController.h"
#include "UMethod.h"

UController::UController(CSHPTRREF<UDevice> _spDevice) : UComponent(_spDevice)
{
}

UController::UController(const UController& _rhs) : UComponent(_rhs)
{
}

HRESULT UController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	return S_OK;
}
