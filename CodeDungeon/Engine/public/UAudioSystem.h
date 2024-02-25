#pragma once
#include "USound.h"
#include "UResource.h"

BEGIN(Engine)
using SOUNDCONTAINER = VECTOR<SHPTR<USound>>;
using SOUNDORDERS = UNORMAP<_wstring, _int>;
using ACTIVESOUND = UNORSET<SHPTR<USound>>;
using REMOVESOUND = LIST<SHPTR<USound>>;

class UAudioSystem final : public UResource {
public:
	UAudioSystem(CSHPTRREF<UDevice> _spDevice);
	UAudioSystem(const UAudioSystem& _rhs);
	DESTRUCTOR(UAudioSystem)
public: /* get set*/
	const SOUNDCONTAINER& GetSoundContainer() const { return m_SoundContainer; }
	const SOUNDORDERS& GetSoundOrders() const { return m_SoundOrders; }
public:
	CLONE_THIS(UAudioSystem)
	virtual void Free() override;
public:
	virtual HRESULT NativeConstruct() override;
	HRESULT NativeConstruct(FMOD::System* _pSystem, const _wstring& _wstrSoudFolderPath);
	HRESULT NativeConstruct(FMOD::System* _pSystem, CSHPTRREF<FILEGROUP> _spSoundFileGroup);
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;

	void Tick();
	void Play(const _wstring& _wstrSoundName);
	void PlayBGM(const _wstring& _wstrSoundName);
	void Stop(const _wstring& _wstrSoundName);

	void UpdateSound3D(const _wstring& _wstrSoundName ,const _float3& _vSoudPos, const _float3& _vSoundVelocity, 
		CSHPTRREF<UTransform> _spTargetTransform_CanNullptr = nullptr);

	void ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance);

	SHPTR<USound> BringSound(const _int _Index);
	SHPTR<USound> BringSound(const _wstring& _wstrSoundName);
private:
	SHPTR<USound> FindActiveSound(const _wstring& _wstrSoundName);
	SHPTR<USound> FIndActvieSound(const _int _Index);
	void LoadSound(const _wstring& _wstrSoundFolderPath);
	void CreateSound(const _wstring& _wstrSoundPath);
private:
	FMOD::System*			m_pSoundSystem;
	FMOD::Channel*			m_pBGmChannel;
	SOUNDCONTAINER		m_SoundContainer;
	SOUNDORDERS			m_SoundOrders;
	ACTIVESOUND				m_ActiveSounds;
	REMOVESOUND			m_RemoveSounds;
};

END