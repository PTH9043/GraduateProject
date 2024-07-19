#pragma once
#include "UScene.h"
BEGIN(Engine)

END

BEGIN(Client)
class CImageUI;
class CButtonUI;

class CLogoScene   final : public UScene {
public:
	CLogoScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CLogoScene)
	DESTRUCTOR(CLogoScene)
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
private:
	SHPTR<CImageUI>			m_spImageUI;
	SHPTR<CButtonUI>			m_spButtonUI;
	SHPTR<CImageUI>			m_spMainTitleUI;
	SHPTR<CImageUI>			m_spBackEffectUI;
};

END

