#pragma once

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include "UBase.h"

BEGIN(Engine)
class UGameInstance;
class UAudioSystem;
class USound;
class UTransform;

using AUDIOSYSTEMCONTAINER = ARRAY <VECTOR<SHPTR< UAudioSystem>>, CLONE_MAX_SIZE>;

class UAudioSystemManager final : public UBase {
public:
	UAudioSystemManager();
	NO_COPY(UAudioSystemManager)
	DESTRUCTOR(UAudioSystemManager)
public:
	HRESULT ReadyAudioSystemManager(UGameInstance* _pGameInstance);
	void Tick();
	void ClearOnceTypeData();

	HRESULT CreateAudioSystem(UGameInstance* _pGameInstance, const _wstring& _wstrProtoTypeTag, CLONETYPE _CloneType, const _wstring& _wstrSoundFolderPath);
	HRESULT CreateAudioSystem(UGameInstance* _pGameInstance, const _wstring& _wstrProtoTypeTag, CLONETYPE _CloneType, CSHPTRREF<FILEGROUP> _spSoundFileGroup);
	HRESULT CreateAudioSystemToFolderName(UGameInstance* _pGameInstance, const _wstring& _wstrProtoTypeTag, CLONETYPE _CloneType, const _wstring& _wstrSoundFolderName);

	void Play(const _wstring& _wstrSoundName);
	void PlayBGM(const _wstring& _wstrSoundName);
	void Stop(const _wstring& _wstrSoundName);

	void UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTransform);
	void ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance);

	SHPTR<USound> BringSound(const _int _Index);
	SHPTR<USound> BringSound(const _wstring& _wstrSoundName);
private:
	virtual void Free() override;
private:
	FMOD::System*						m_pSystem;
	AUDIOSYSTEMCONTAINER	m_AudioSystemContainer;
};


END