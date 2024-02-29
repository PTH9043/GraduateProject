#pragma once
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include "UBase.h"

BEGIN(Engine)
class UAudioSystem;
class UTransform;
/*
@ Date: 2024-02-25, Writer: 박태현
@ Explain
- Sound를 파일에서 로드하고 담당하는 클래스
*/
class USound final : public UBase {
public:
	USound(FMOD::System* _pSystem, const _wstring& _wstrSoundPath, _int _SoundIndex);
	DESTRUCTOR(USound)
public: /* get set */
	SOUNDTYPE GetSoundType() const { return m_SoundDesc.SoundType; }
	_float GetVolume() const { return m_SoundDesc.fVolume; }
	FMOD::Sound* GetSound() const { return m_pSound; }
	const _bool IsSoundPlay() const { return m_isSoundPlay; }

public:
	virtual void Free() override;
	void Tick();
	void Play();
	void PlayBGM(IN FMOD::Channel** _ppChannel);
	void Stop();
	void UpdateVolume(const _float _fVolume) ;
	void UpdateSound3D(const _float3& _vSoudPos, const _float3& _vSoundVelocity, CSHPTRREF<UTransform> _spTargetTransform_CanNullptr = nullptr);
	void UpdateSound3D(CSHPTRREF<UTransform> _spSelfTransform, const _float3& _vSoundVelocity,
		CSHPTRREF<UTransform> _spTargetTransform_CanNullptr = nullptr);
	void ChangeMinMaxDistance3D(const _float _fMinDistance, const _float _fMaxDistance);
private:
	_int							m_iIndex;
	SOUNDDESC			m_SoundDesc;
	_bool						m_isSoundPlay;

	FMOD_VECTOR		m_SoundPos;
	FMOD_VECTOR		m_SoundVelocity;

	FMOD_VECTOR		m_ListenerPos;
	FMOD_VECTOR		m_ListenerLook;
	FMOD_VECTOR		m_ListenerUp;

	FMOD::System*	m_pSystem;
	FMOD::Sound*		m_pSound;
	FMOD::Channel*  m_pChannel;
};

/* 
=============================================
Sound
=============================================
*/

END
