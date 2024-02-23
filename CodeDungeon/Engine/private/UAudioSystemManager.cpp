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
		for (auto& Sound : iter)
		{
			Sound->Tick();
		}
	}
}

void UAudioSystemManager::ClearOnceTypeData()
{
	_int iCloneType = static_cast<_int>(CLONETYPE::CLONE_ONCE);
	m_AudioSystemContainer[iCloneType].clear();
}

HRESULT UAudioSystemManager::CreateAudioSystem(UGameInstance* _pGameInstance, const _wstring& _wstrProtoTypeTag, CLONETYPE _CloneType, 
	const _wstring& _wstrSoundFolderPath)
{
	_int iCloneType = static_cast<_int>(_CloneType);
	RETURN_CHECK(iCloneType >= MAX_CAM_SIZE, E_FAIL);
	SHPTR<UAudioSystem> spAudioSystem{ CreateConstructorNative< UAudioSystem>(_pGameInstance->GetDevice(), m_pSystem, _wstrSoundFolderPath) };
	m_AudioSystemContainer[iCloneType].emplace_back(spAudioSystem);
	return S_OK;
}

HRESULT UAudioSystemManager::CreateAudioSystem(UGameInstance* _pGameInstance, const _wstring& _wstrProtoTypeTag, CLONETYPE _CloneType, 
	CSHPTRREF<FILEGROUP> _spSoundFileGroup)
{
	_int iCloneType = static_cast<_int>(_CloneType);
	RETURN_CHECK(iCloneType >= MAX_CAM_SIZE, E_FAIL);
	SHPTR<UAudioSystem> spAudioSystem{ CreateConstructorNative< UAudioSystem>(_pGameInstance->GetDevice(), m_pSystem, _spSoundFileGroup) };
	m_AudioSystemContainer[iCloneType].emplace_back(spAudioSystem);
	return S_OK;
}

HRESULT UAudioSystemManager::CreateAudioSystemToFolderName(UGameInstance* _pGameInstance, const _wstring& _wstrProtoTypeTag, CLONETYPE _CloneType, 
	const _wstring& _wstrSoundFolderName)
{
	SHPTR<FILEGROUP> spAudioFolder = _pGameInstance->FindFolder(_wstrSoundFolderName);
	return CreateAudioSystem(_pGameInstance, _wstrProtoTypeTag, _CloneType, spAudioFolder);
}

void UAudioSystemManager::Play(const _wstring& _wstrSoundName)
{
}

void UAudioSystemManager::PlayBGM(const _wstring& _wstrSoundName)
{
}

void UAudioSystemManager::Stop(const _wstring& _wstrSoundName)
{
}

void UAudioSystemManager::UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTransform)
{
}

void UAudioSystemManager::ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance)
{
}

SHPTR<USound> UAudioSystemManager::BringSound(const _int _Index)
{
	return SHPTR<USound>();
}

SHPTR<USound> UAudioSystemManager::BringSound(const _wstring& _wstrSoundName)
{
	return SHPTR<USound>();
}

void UAudioSystemManager::Free()
{
	if (m_pSystem){
		m_pSystem->release();
	}

	for (auto& iter : m_AudioSystemContainer)
		iter.clear();
}
