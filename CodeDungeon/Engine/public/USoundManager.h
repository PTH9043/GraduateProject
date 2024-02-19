#pragma once
#include "USound.h"
#include "UBase.h"

BEGIN(Engine)

/*
@ Date: 2024-02-18, Writer: 박태현
@ Explain
- Sound를 관리하는 SoundManager
*/
class USoundManager final : public UBase {
public:
	USoundManager();
	NO_COPY(USoundManager)
	DESTRUCTOR(USoundManager)
public:
	virtual void Free() override;
	HRESULT ReadySoundManager();
private:
	FMOD::System*		m_pSystem;
};


END