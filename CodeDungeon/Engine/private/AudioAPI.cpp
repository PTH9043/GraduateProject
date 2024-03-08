#include "EngineDefine.h"
#include "AudioAPI.h"

USound::USound(FMOD::System* _pSystem, const _string& _strSoundPath, SOUNDTYPE _SoundType) : 
	m_SoundType{_SoundType}, m_pSound{nullptr}
{
	CreateSound(_pSystem, _strSoundPath);
}

void USound::CreateSound(FMOD::System* _pSystem, const _string& _strSoundPath)
{
	assert(nullptr != _pSystem);
	FMOD_MODE Mode = ComputeMode();

	FMOD_RESULT Result = _pSystem->createSound(_strSoundPath.c_str(), Mode, nullptr, &m_pSound);
	assert(FMOD_OK == Result);
}

FMOD_MODE USound::ComputeMode()
{
	FMOD_MODE Mode = FMOD_DEFAULT;
	switch (m_SoundType)
	{
	case SOUND_2D:
		Mode |= FMOD_2D;
		break;
	case SOUND_3D:
		Mode |= FMOD_3D;
		break;
	}

	return Mode;
}

void USound::Free()
{
	m_pSound->release();
}

/*
=============================================
Sound
=============================================
Channel
=============================================
*/
