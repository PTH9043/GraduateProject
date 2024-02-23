#include "EngineDefine.h"
#include "UAudioSystemManager.h"
#include "UGameInstance.h"
#include "UAudioSystem.h"
#include "UAudioSystem.h"

UAudioSystemManager::UAudioSystemManager() : m_AudioSystemContainer{}, m_pSystem { nullptr }
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

void UAudioSystemManager::Tick()
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if(nullptr != iter)
			iter->Tick();
	}
}

void UAudioSystemManager::ClearOnceTypeData()
{
	_int iCloneType = static_cast<_int>(CLONETYPE::CLONE_ONCE);
	m_AudioSystemContainer[iCloneType].reset();
}

HRESULT UAudioSystemManager::CreateAudioSystemAndRegister(UGameInstance* _pGameInstance, CLONETYPE _CloneType,
	const _wstring& _wstrSoundFolderPath)
{
	_int iCloneType = static_cast<_int>(_CloneType);
	RETURN_CHECK(iCloneType >= MAX_CAM_SIZE, E_FAIL);
	assert(nullptr == m_AudioSystemContainer[iCloneType]);
	SHPTR<UAudioSystem> spAudioSystem{ CreateConstructorNative< UAudioSystem>(_pGameInstance->GetDevice(), m_pSystem, _wstrSoundFolderPath) };
	m_AudioSystemContainer[iCloneType] = spAudioSystem;
	return S_OK;
}

HRESULT UAudioSystemManager::CreateAudioSystemAndRegister(UGameInstance* _pGameInstance, CLONETYPE _CloneType,
	CSHPTRREF<FILEGROUP> _spSoundFileGroup)
{
	_int iCloneType = static_cast<_int>(_CloneType);
	RETURN_CHECK(iCloneType >= MAX_CAM_SIZE, E_FAIL);
	assert(nullptr == m_AudioSystemContainer[iCloneType]);
	SHPTR<UAudioSystem> spAudioSystem{ CreateConstructorNative< UAudioSystem>(_pGameInstance->GetDevice(), m_pSystem, _spSoundFileGroup) };
	m_AudioSystemContainer[iCloneType] = spAudioSystem;
	return S_OK;
}

HRESULT UAudioSystemManager::CreateAudioSystemToFolderNameAndRegister(UGameInstance* _pGameInstance,  CLONETYPE _CloneType,
	const _wstring& _wstrSoundFolderName)
{
	SHPTR<FILEGROUP> spAudioFolder = _pGameInstance->FindFolder(_wstrSoundFolderName);
	return CreateAudioSystemAndRegister(_pGameInstance, _CloneType, spAudioFolder);
}

void UAudioSystemManager::Play(const _wstring& _wstrSoundName)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->Play(_wstrSoundName);
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

void UAudioSystemManager::Stop(const _wstring& _wstrSoundName)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if(nullptr != iter)
			iter->Stop(_wstrSoundName);
	}
}

void UAudioSystemManager::UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTransform)
{
	for (auto& iter : m_AudioSystemContainer)
	{
		if (nullptr != iter)
			iter->UpdateSound3D(_wstrSoundName, _vSoudPos, _vSoundVelocity, _spTransform);
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
	if (m_pSystem) {
		m_pSystem->release();
	}
}
