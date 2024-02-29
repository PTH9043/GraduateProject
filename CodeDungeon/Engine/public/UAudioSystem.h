#pragma once
#include "USound.h"
#include "UResource.h"

BEGIN(Engine)
using SOUNDCONTAINER = VECTOR<SHPTR<USound>>;
using SOUNDORDERS = UNORMAP<_wstring, _int>;
using ACTIVESOUND = UNORSET<SHPTR<USound>>;
using REMOVESOUND = LIST<SHPTR<USound>>;
/*
@ Date: 2024-02-25, Writer: 박태현
@ Explain
- Sound 폴더를 받아와서 Sound 전체를 로드해서 관리하는 클래스이다. 
*/
class UAudioSystem final : public UResource {
public:
	UAudioSystem(CSHPTRREF<UDevice> _spDevice);
	UAudioSystem(const UAudioSystem& _rhs);
	DESTRUCTOR(UAudioSystem)
public: /* get set*/
	const SOUNDCONTAINER& GetSoundContainer() const { return m_SoundContainer; }
	const SOUNDORDERS& GetSoundOrders() const { return m_SoundOrders; }

	CSHPTRREF<USound> GetSound() const { return m_spSound; }
	CSHPTRREF<USound> GetBGMSound() const { return m_spBgmSound; }
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
	void Play(const _wstring& _wstrSoundName, const _float _fVolumeUpdate);
	void PlayBGM(const _wstring& _wstrSoundName);
	void PlayBGM(const _wstring& _wstrSoundName, const _float  _fVolumeUpdate);
	void Stop(const _wstring& _wstrSoundName);
	void VolumeUpdate(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate);

	void UpdateSound3D(const _wstring& _wstrSoundName ,const _float3& _vSoudPos, const _float3& _vSoundVelocity, 
		CSHPTRREF<UTransform> _spTargetTransform_CanNullptr = nullptr);
	void UpdateSound3D(const _wstring& _wstrSoundName, CSHPTRREF<UTransform> _spSelfTransform, const _float3& _vSoundVelocity,
		CSHPTRREF<UTransform> _spTargetTransform_CanNullptr = nullptr);
	void ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance);

	SHPTR<USound> BringSound(const _int _Index);
	SHPTR<USound> BringSound(const _wstring& _wstrSoundName);

	void Release();
private:
	void LoadSound(const _wstring& _wstrSoundFolderPath);
	void CreateSound(const _wstring& _wstrSoundPath);
private:
	FMOD::System*			m_pSoundSystem;
	FMOD::Channel*			m_pBGmChannel;
	SOUNDCONTAINER		m_SoundContainer;
	SOUNDORDERS			m_SoundOrders;

	SHPTR<USound>			m_spSound;
	SHPTR<USound>			m_spBgmSound;
};

END