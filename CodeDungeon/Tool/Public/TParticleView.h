#pragma once

#include "TImGuiView.h"

BEGIN(Engine)
class UParticle;
END

BEGIN(Tool)

class TParticleView : public TImGuiView {
public:
	TParticleView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TParticleView)
		DESTRUCTOR(TParticleView)
public:
	// TImGuiView을(를) 통해 상속됨
	virtual void Free() override;

	virtual HRESULT NativeConstruct() override;
protected:
	virtual HRESULT LoadResource() override;
	virtual HRESULT ReleaseResource() override;
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDetla) override;
	virtual void RenderActive() override;
private:
	void RenderMenu();
	void DockBuildInitSetting();

	void ParticleView();
	void SetParticle(SHPTR<UParticle> _spParticle) { m_spParticle = _spParticle; }
private:
	MAINDESC													m_stMainDesc;
	DOCKDESC												m_stParticleView;

	VECTOR<SHPTR<TImGuiView>>			m_CloseImGuies;
	SET<SHPTR<TImGuiView>>					m_OpenImGuies;

	_double														m_dShowDeltaTime;
	_bool															m_isInitSetting;

	SHPTR<UParticle>		m_spParticle;
	_bool														m_isResetParticle;
};


END