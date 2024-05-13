#pragma once
#include "UScene.h"
BEGIN(Engine)
class UFire;
class UParticle;
END
BEGIN(Client)

class CMainCamera;
class CMap;
class CWarriorPlayer;


class CMainScene final : public UScene{
public:
	CMainScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CMainScene)
	DESTRUCTOR(CMainScene)
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
private:
	// Main
	SHPTR<CMainCamera>			m_spMainCamera;

	SHPTR<CMap>							m_spMap;
	SHPTR< CWarriorPlayer>		m_spWarriorPlayer;

	SHPTR<UFire> m_stFire;
	FIRENOISEBUFFER* m_stFireNoiseBuffer;
	FIREDISTORTIONBUFFER* m_stFireDistortionBuffer;

};

END

