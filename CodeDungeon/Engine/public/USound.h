#pragma once
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include <fmod/fmod.h>
#include "UBase.h"


BEGIN(Engine)
class UTransform;

class USound final : public UBase {
public:
	USound();
	NO_COPY(USound)
	DESTRUCTOR(USound)
public: /* Get Set*/
	const _float GetVolume() const { return m_fVolume; }
	FMOD::Sound* GetSound() const { return m_pSound; }

	void SetVolume(const _float _fVolume) { this->m_fVolume = _fVolume; }
public:
	HRESULT NativeConstruct(FMOD::System* _pSystem, const _string& _strSoundPath);
	void VolumeUp(const _float _fVol);
	void VolumeDown(const _float _fVol);
	void Play();
	void Stop();

private:
	virtual void Free() override;
private:
	static constexpr _float		MAX_SOUND{ 1.f };
	static constexpr _float		MIN_SOUND{ 0.f };

	_float										m_fVolume;
	FMOD::Sound*						m_pSound;
};


END
