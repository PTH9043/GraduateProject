#include "EngineDefine.h"
#include "UController.h"

UController::UController(CSHPTRREF<UDevice> _spDevice) : UComponent(_spDevice)
{
}

UController::UController(const UController& _rhs) : UComponent(_rhs)
{
}
