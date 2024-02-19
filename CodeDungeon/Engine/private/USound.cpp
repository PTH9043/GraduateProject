#include "EngineDefine.h"
#include "USound.h"

USound::USound() : 
	m_fVolume{0.5f},
	m_pSound{nullptr}
{
}

HRESULT USound::NativeConstruct(FMOD::System* _pSystem, const _string& _strSoundPath)
{
	FMOD_CREATESOUNDEXINFO Info;
	_pSystem->createSound(_strSoundPath.c_str(), FMOD_3D, &Info, &m_pSound);
	assert(nullptr != m_pSound);
	return S_OK;
}

void USound::VolumeUp(const _float _fVol)
{

}

void USound::VolumeDown(const _float _fVol)
{
}

void USound::Play()
{
	
}

void USound::Stop()
{
}

void USound::Free()
{
}
