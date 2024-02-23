#include "EngineDefine.h"
#include "UAudioSystem.h"
#include "UGameInstance.h"
namespace fs = std::filesystem;

UAudioSystem::UAudioSystem(CSHPTRREF<UDevice> _spDevice) : 
	UResource(_spDevice), 
	m_pSoundSystem{nullptr},m_pBGmChannel{nullptr}, 
	m_SoundContainer{},	m_SoundOrders{}, m_ActiveSounds{}, m_RemoveSounds{}
{
}

UAudioSystem::UAudioSystem(const UAudioSystem& _rhs) 
	: UResource(_rhs),
	m_pSoundSystem{ nullptr }, m_pBGmChannel{ nullptr },
	m_SoundContainer{}, m_SoundOrders{}, m_ActiveSounds{}, m_RemoveSounds{}
{
}

void UAudioSystem::Free()
{
}

HRESULT UAudioSystem::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UAudioSystem::NativeConstruct(FMOD::System* _pSystem, const _wstring& _wstrSoudFolderPath)
{
	RETURN_CHECK(true == _wstrSoudFolderPath.empty()  || nullptr == _pSystem, E_FAIL);
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	m_pSoundSystem = _pSystem;
	LoadSound(_wstrSoudFolderPath);
	return S_OK;
}

HRESULT UAudioSystem::NativeConstruct(FMOD::System* _pSystem, CSHPTRREF<FILEGROUP> _spSoundFileGroup)
{
	RETURN_CHECK(nullptr == _spSoundFileGroup || nullptr == _pSystem, E_FAIL);
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	m_pSoundSystem = _pSystem;
	for (auto& iter : _spSoundFileGroup->FileDataList) {
		CreateSound(iter.second->wstrfilePath);
	}
	return S_OK;
}


HRESULT UAudioSystem::NativeConstructClone(const VOIDDATAS& _Datas)
{
	return __super::NativeConstructClone(_Datas);
}

void UAudioSystem::Tick()
{
	for (auto& iter : m_ActiveSounds)
	{
		iter->Tick(this);
		if (false == iter->IsSoundPlay())
			m_RemoveSounds.push_back(iter);
	}

	for (auto& iter : m_RemoveSounds)
		m_ActiveSounds.erase(iter);

	m_RemoveSounds.clear();
}

void UAudioSystem::Play(const _wstring& _wstrSoundName)
{
	SHPTR<USound> spSound = BringSound(_wstrSoundName);
	assert(nullptr != spSound);
	spSound->Play();
	m_ActiveSounds.insert(spSound);
}

void UAudioSystem::PlayBGM(const _wstring& _wstrSoundName)
{
	SHPTR<USound> spSound = BringSound(_wstrSoundName);
	assert(nullptr != spSound);
	spSound->PlayBGM(&m_pBGmChannel);
}

void UAudioSystem::Stop(const _wstring& _wstrSoundName)
{
	SHPTR<USound> spSound = BringSound(_wstrSoundName);
	assert(nullptr != spSound);
	spSound->Stop();
	m_ActiveSounds.erase(spSound);
}

void UAudioSystem::UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTransform)
{
	CSHPTRREF<USound> spSound = FindActiveSound(_wstrSoundName);
	spSound->UpdateSound3D(_vSoudPos, _vSoundVelocity, _spTransform);
}

void UAudioSystem::ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance)
{
	CSHPTRREF<USound> spSound = FindActiveSound(_wstrSoundName);
	spSound->ChangeMinMaxDistance3D(_fMinDistance, _fMaxDistance);
}

SHPTR<USound> UAudioSystem::BringSound(const _int _Index)
{
	RETURN_CHECK(_Index < 0 || _Index >= m_SoundContainer.size(), nullptr)
	return m_SoundContainer[_Index];
}

SHPTR<USound> UAudioSystem::BringSound(const _wstring& _wstrSoundName)
{
	const auto& FindIter = m_SoundOrders.find(_wstrSoundName);
	RETURN_CHECK(m_SoundOrders.end() == FindIter, nullptr);
	return m_SoundContainer[FindIter->second];
}

SHPTR<USound> UAudioSystem::FindActiveSound(const _wstring& _wstrSoundName)
{
	RETURN_CHECK(m_SoundOrders.end() == m_SoundOrders.find(_wstrSoundName), nullptr);
	CSHPTRREF<USound> spSound = m_SoundContainer[m_SoundOrders.find(_wstrSoundName)->second];
	const auto& FindSound = m_ActiveSounds.find(spSound);
	RETURN_CHECK(m_ActiveSounds.end() == FindSound, nullptr);
	return spSound;
}

SHPTR<USound> UAudioSystem::FIndActvieSound(const _int _Index)
{
	RETURN_CHECK(_Index < 0 || _Index >= m_SoundContainer.size(), nullptr)
	CSHPTRREF<USound> spSound = m_SoundContainer[_Index];
	const auto& FindSound = m_ActiveSounds.find(spSound);
	RETURN_CHECK(m_ActiveSounds.end() == FindSound, nullptr);
	return spSound;
}

void UAudioSystem::LoadSound(const _wstring& _wstrSoundFolderPath)
{
	fs::directory_iterator end;
	for (fs::directory_iterator iter(_wstrSoundFolderPath); iter != end; ++iter)
	{
		// RegularFile
		if (fs::is_regular_file(iter->status()))
		{
			CreateSound(iter->path());
		}
		else if (fs::is_directory(iter->status()))
		{
			LoadSound(iter->path());
		}
	}
}

void UAudioSystem::CreateSound(const _wstring& _wstrSoundPath)
{
	_wstring Name = _wstrSoundPath.substr(_wstrSoundPath.find_last_of(L"\\") + 1, _wstrSoundPath.find_last_of(L"."));
	Name = Name.substr(0, Name.find_last_of(L"."));
	_int SoundIndex = static_cast<_int>(m_SoundContainer.size());
	m_SoundOrders.emplace(MakePair(Name, SoundIndex));
	m_SoundContainer.push_back(Create<USound>(m_pSoundSystem, _wstrSoundPath, SoundIndex));
}
