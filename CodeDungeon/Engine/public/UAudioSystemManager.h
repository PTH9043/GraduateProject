#pragma once

#include "AudioAPI.h"
#include "UBase.h"

BEGIN(Engine)
class UGameInstance;

class UAudioSystemManager final : public UBase {
public:
	UAudioSystemManager();
	NO_COPY(UAudioSystemManager)
	DESTRUCTOR(UAudioSystemManager)
public:
	HRESULT ReadyAudioSystemManager(UGameInstance* _pGameInstance);
private:
	virtual void Free() override;
private:
	FMOD::System*		m_pSystem;
};


END