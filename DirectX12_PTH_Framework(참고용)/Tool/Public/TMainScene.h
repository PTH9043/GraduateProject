#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_TMAINSCENE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_TMAINSCENE_H

#include "UScene.h"

BEGIN(Engine)

END

BEGIN(Tool)

class TMainCamera;

class TMainScene : public UScene{
public:
	TMainScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TMainScene)
	DESTRUCTOR(TMainScene)
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
private:
	// Main
	SHPTR<TMainCamera>			m_spMainCamera;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_TMAINSCENE_H