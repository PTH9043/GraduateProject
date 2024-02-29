#include "EngineDefine.h"
#include "UAudioSystemManager.h"
#include "UGameInstance.h"
#include "UAudioSystem.h"
#include "UTransform.h"

UAudioSystemManager::UAudioSystemManager() : m_AudioSystemContainer{}, m_pSystem { nullptr }
{
	for (auto& iter : m_AudioSystemContainer)
		iter = nullptr;
}

SHPTR<UAudioSystem> UAudioSystemManager::GetAudioSystem(const SOUNDTYPE _SoundType)
{
	RETURN_CHECK(_SoundType >= SOUND_END, nullptr);
	const auto& iter = m_AudioSystemContainer[_SoundType];
	return iter;
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

HRESULT UAudioSystemManager::CreateAudioSystemAndRegister(UGameInstance* _pGameInstance, SOUNDTYPE _SoundType,
	const _wstring& _wstrSoundFolderPath)
{
	RETURN_CHECK(_SoundType >= SOUND_END, E_FAIL);
	assert(nullptr == m_AudioSystemContainer[_SoundType]);
	SHPTR<UAudioSystem> spAudioSystem{ CreateConstructorNative< UAudioSystem>(_pGameInstance->GetDevice(), m_pSystem, _wstrSoundFolderPath) };
	m_AudioSystemContainer[_SoundType] = spAudioSystem;
	return S_OK;
}

HRESULT UAudioSystemManager::CreateAudioSystemAndRegister(UGameInstance* _pGameInstance, SOUNDTYPE _SoundType,
	CSHPTRREF<FILEGROUP> _spSoundFileGroup)
{
	RETURN_CHECK(_SoundType >= SOUND_END, E_FAIL);
	assert(nullptr == m_AudioSystemContainer[_SoundType]);
	SHPTR<UAudioSystem> spAudioSystem{ CreateConstructorNative< UAudioSystem>(_pGameInstance->GetDevice(), m_pSystem, _spSoundFileGroup) };
	m_AudioSystemContainer[_SoundType] = spAudioSystem;
	return S_OK;
}

HRESULT UAudioSystemManager::CreateAudioSystemToFolderNameAndRegister(UGameInstance* _pGameInstance, SOUNDTYPE _SoundType,
	const _wstring& _wstrSoundFolderName)
{
	SHPTR<FILEGROUP> spAudioFolder = _pGameInstance->FindFolder(_wstrSoundFolderName);
	return CreateAudioSystemAndRegister(_pGameInstance, _SoundType, spAudioFolder);
}

void UAudioSystemManager::Play(const _wstring& _wstrSoundName)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->Play(_wstrSoundName);
	}
}

void UAudioSystemManager::Play(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->Play(_wstrSoundName, _fVolumeUpdate);
	}
}

void UAudioSystemManager::PlayBGM(const _wstring& _wstrSoundName)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->PlayBGM(_wstrSoundName);
	}
}

void UAudioSystemManager::PlayBGM(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->PlayBGM(_wstrSoundName, _fVolumeUpdate);
	}
}

void UAudioSystemManager::Stop(const _wstring& _wstrSoundName)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if(nullptr != iter)
			iter->Stop(_wstrSoundName);
	}
}

void UAudioSystemManager::VolumeUpdate(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->VolumeUpdate(_wstrSoundName, _fVolumeUpdate);
	}
}

void UAudioSystemManager::UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTargetTransform)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->UpdateSound3D(_wstrSoundName, _vSoudPos, _vSoundVelocity, _spTargetTransform);
	}
}

void UAudioSystemManager::UpdateSound3D(const _wstring& _wstrSoundName, CSHPTRREF<UTransform> _spSoundTransform, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTargetTransform)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->UpdateSound3D(_wstrSoundName, _spSoundTransform, _vSoundVelocity, _spTargetTransform);
	}
}

void UAudioSystemManager::ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->ChangeMinMaxDistance3D(_wstrSoundName, _fMinDistance, _fMaxDistance);
	}
}

SHPTR<USound> UAudioSystemManager::BringSound(const _int _Index)
{
	SHPTR<USound> spSound{ nullptr };
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			spSound = iter->BringSound(_Index);
	}
	return spSound;
}

SHPTR<USound> UAudioSystemManager::BringSound(const _wstring& _wstrSoundName)
{
	SHPTR<USound> spSound{ nullptr };
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			spSound = iter->BringSound(_wstrSoundName);
	}
	return spSound;
}

void UAudioSystemManager::Free()
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->Release();

		iter.reset();
	}

	if (m_pSystem) {
		m_pSystem->release();
	}
}
