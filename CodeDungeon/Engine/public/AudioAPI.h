#pragma once
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include "UBase.h"

BEGIN(Engine)

class USound abstract : public UBase {
public:
	USound(FMOD::System* _pSystem, const _string& _strSoundPath, SOUNDTYPE _SoundType);
	DESTRUCTOR(USound)
public: /* get set */
	SOUNDTYPE GetSoundType() const { return m_SoundType; }
	FMOD::Sound* GetSound() const { return m_pSound; }
public:

private:
	void CreateSound(FMOD::System* _pSystem, const _string& _strSoundPath);
	FMOD_MODE ComputeMode();
	virtual void Free() override;
private:
	SOUNDTYPE			m_SoundType;
	_float						m_fVolume;
	FMOD::Sound*		m_pSound;
};

/* 
=============================================
Sound
=============================================
Channel
=============================================
*/

END
