#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CSTAGE2SCENE_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CSTAGE2SCENE_H

#include "UScene.h"

BEGIN(Engine)
class UTerrain;
class UMirror;
class USkyBox;
END

BEGIN(Client)
class CPlayer;
class CMainCamera;

class CStage2Scene  final : public UScene{
public:
	CStage2Scene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CStage2Scene)
	DESTRUCTOR(CStage2Scene)
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
	virtual void CollisionTick(const _double& _dTimedelta) override;
private:
	SHPTR<USkyBox>					m_spSkyBox;
	SHPTR<CMainCamera>						m_spMainCamera;
	SHPTR<CPlayer>									m_spPlayer;
	VECTOR<SHPTR< UMirror>>				m_MirrorContainer;
	SHPTR< UTerrain>								m_spTerrainObject;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CSTAGE2SCENE_H
