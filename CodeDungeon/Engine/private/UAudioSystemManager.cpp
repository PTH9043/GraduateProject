#include "EngineDefine.h"
#include "UAudioSystemManager.h"

UAudioSystemManager::UAudioSystemManager() : m_pSystem{nullptr}
{
}

HRESULT UAudioSystemManager::ReadyAudioSystemManager(UGameInstance* _pGameInstance)
{
	// Create System 
	FMOD_RESULT Result = FMOD::System_Create(&m_pSystem);
	RETURN_CHECK(FMOD_OK != Result, E_FAIL);
	// Init 
	Result = m_pSystem->init(MAX_CHANNEL_CNT, FMOD_INIT_NORMAL, 0);
	RETURN_CHECK(FMOD_OK != Result, E_FAIL);
	return S_OK;
}

void UAudioSystemManager::Free()
{
	if (m_pSystem){
		m_pSystem->release();
	}
}
