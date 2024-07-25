#include "EngineDefine.h"
#include "UAudioSystem.h"
#include "UGameInstance.h"
namespace fs = std::filesystem;

UAudioSystem::UAudioSystem(CSHPTRREF<UDevice> _spDevice) : 
	UResource(_spDevice), 
	m_pSoundSystem{nullptr},m_pBGmChannel{nullptr}, 
	m_SoundContainer{},	m_SoundOrders{}, 
	m_spSound{nullptr}, m_spBgmSound{nullptr}
{
}

UAudioSystem::UAudioSystem(const UAudioSystem& _rhs) 
	: UResource(_rhs),
	m_pSoundSystem{ nullptr }, m_pBGmChannel{ nullptr },
	m_SoundContainer{}, m_SoundOrders{}, 
	m_spSound{ nullptr }, m_spBgmSound{nullptr}
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
	for (auto& iter : m_SoundContainer)
	{
		iter->Tick();
	}
}

void UAudioSystem::Play(const _wstring& _wstrSoundName)
{
	m_spSound = BringSound(_wstrSoundName);
	assert(nullptr != m_spSound);
	m_spSound->Play();
}

void UAudioSystem::Play(const _wstring& _wstrSoundName, const _float _fVolumeUpdate)
{
	m_spSound = BringSound(_wstrSoundName);
	assert(nullptr != m_spSound);
	m_spSound->Play();
	m_spSound->UpdateVolume(_fVolumeUpdate);
}

void UAudioSystem::PlayBGM(const _wstring& _wstrSoundName)
{
	m_spBgmSound = BringSound(_wstrSoundName);
	assert(nullptr != m_spBgmSound);
	m_spBgmSound->PlayBGM(&m_pBGmChannel);
}

void UAudioSystem::PlayBGM(const _wstring& _wstrSoundName, const _float _fVolumeUpdate)
{
	m_spBgmSound = BringSound(_wstrSoundName);
	assert(nullptr != m_spBgmSound);
	m_spBgmSound->PlayBGM(&m_pBGmChannel);
	m_spBgmSound->UpdateVolume(_fVolumeUpdate);
}

void UAudioSystem::Stop(const _wstring& _wstrSoundName)
{
	m_spSound = BringSound(_wstrSoundName);
	assert(nullptr != m_spSound);
	m_spSound->Stop();
}

void UAudioSystem::VolumeUpdate(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate)
{
	m_spSound = BringSound(_wstrSoundName);
	assert(nullptr != m_spSound);
	m_spSound->UpdateVolume(_fVolumeUpdate);
}

void UAudioSystem::UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, 
	const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTargetTransform_CanNullptr)
{
	CSHPTRREF<USound> spSound = BringSound(_wstrSoundName);
	spSound->UpdateSound3D(_vSoudPos, _vSoundVelocity, _spTargetTransform_CanNullptr);
}

void UAudioSystem::UpdateSound3D(const _wstring& _wstrSoundName, CSHPTRREF<UTransform> _spSelfTransform, const _float3& _vSoundVelocity,
	CSHPTRREF<UTransform> _spTargetTransform_CanNullptr)
{
	CSHPTRREF<USound> spSound = BringSound(_wstrSoundName);
	spSound->UpdateSound3D(_spSelfTransform, _vSoundVelocity, _spTargetTransform_CanNullptr);
}

void UAudioSystem::ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance)
{
	CSHPTRREF<USound> spSound = BringSound(_wstrSoundName);
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

void UAudioSystem::Release()
{
	for (auto& iter : m_SoundContainer)
		iter->Free();
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
