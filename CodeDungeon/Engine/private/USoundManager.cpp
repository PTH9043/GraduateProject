#include "EngineDefine.h"
#include "USoundManager.h"

USoundManager::USoundManager() : m_pSystem{nullptr}
{
}

void USoundManager::Free()
{
}

HRESULT USoundManager::ReadySoundManager()
{
	FMOD::System_Create(&m_pSystem);
	return S_OK;
}
