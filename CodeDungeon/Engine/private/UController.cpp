#include "EngineDefine.h"
#include "UController.h"

UController::UController(CSHPTRREF<UDevice> _spDevice) : UComponent(_spDevice), m_iControllerOrder{0}
{
}

UController::UController(const UController& _rhs) : UComponent(_rhs), m_iControllerOrder{0}
{
}
