#include "EngineDefine.h"
#include "USound.h"
#include "UAudioSystem.h"
#include "UTransform.h"
#include "UMethod.h"

USound::USound(FMOD::System* _pSystem, const _wstring& _wstrSoundPath, _int _SoundIndex) :
	m_iIndex{ _SoundIndex }, m_SoundDesc{ SOUND_END, 0.5f }, m_isSoundPlay{ false },
	m_SoundPos{}, m_SoundVelocity{}, m_ListenerPos{}, m_ListenerLook{}, m_ListenerUp{}, 
	m_pSystem{_pSystem}, m_pSound{nullptr}, m_pChannel{nullptr}
{
	assert(nullptr != m_pSystem);
	FMOD_RESULT Result = m_pSystem->createSound(UMethod::ConvertWToS(_wstrSoundPath).c_str(), FMOD_DEFAULT, nullptr, &m_pSound);
	assert(FMOD_OK == Result);
}

void USound::Tick(UAudioSystem* _pAudioSystem)
{
	RETURN_CHECK(nullptr == m_pChannel, ;);
	m_pChannel->isPlaying(&m_isSoundPlay);
	if (false == m_isSoundPlay){
		m_pChannel = nullptr;
		m_isSoundPlay = false;
	}
}

void USound::Play()
{
	Stop();
	m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
	m_pChannel->setVolume(m_SoundDesc.fVolume);
	m_SoundDesc.SoundType = SOUND_3D;
}

void USound::PlayBGM(IN FMOD::Channel** _ppChannel)
{
	assert(nullptr != _ppChannel);
	m_pSystem->playSound(m_pSound, nullptr, false, _ppChannel);
	(*_ppChannel)->setMode(FMOD_LOOP_NORMAL);
	(*_ppChannel)->setVolume(m_SoundDesc.fVolume);
	m_pSystem->update();
	m_SoundDesc.SoundType = SOUND_BACKGROUND;
}

void USound::Stop()
{
	RETURN_CHECK(nullptr == m_pChannel, ;);
	m_pChannel->stop();
	m_pChannel = nullptr;
}

void USound::UpdateSound3D(const _float3& _vSoudPos, const _float3& _vSoundVelocity, 
	CSHPTRREF<UTransform> _spTransform)
{
	assert(SOUND_3D != m_SoundDesc.SoundType);
	RETURN_CHECK(nullptr == m_pChannel, ;);
	// Sound Pos, Velocity Update
	{
		::memcpy(&m_SoundPos, &_vSoudPos, GetTypeSize<_float3>());
		::memcpy(&m_SoundVelocity, &_vSoundVelocity, GetTypeSize<_float3>());
		m_pChannel->set3DAttributes(&m_SoundPos, &m_SoundVelocity);
	}
	// Listener Update
	{
		::memcpy(&m_ListenerPos, &_spTransform->GetPos(), GetTypeSize<_float3>());
		::memcpy(&m_ListenerLook, &_spTransform->GetLook(), GetTypeSize<_float3>());
		::memcpy(&m_ListenerUp, &_spTransform->GetUp(), GetTypeSize<_float3>());
		m_pSystem->set3DListenerAttributes(0, &m_ListenerPos, nullptr, &m_ListenerLook, &m_ListenerUp);
	}
	m_pSystem->update();
}

void USound::ChangeMinMaxDistance3D(const _float _fMinDistance, const _float _fMaxDistance)
{
	assert(SOUND_3D != m_SoundDesc.SoundType);
	m_pSound->set3DMinMaxDistance(_fMinDistance, _fMaxDistance);
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
