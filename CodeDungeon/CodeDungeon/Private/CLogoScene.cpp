#include "ClientDefines.h"
#include "CLogoScene.h"

BEGIN(Client)

CLogoScene::CLogoScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice, SCENE_LOGO)
{
}

void CLogoScene::Free()
{
}

HRESULT CLogoScene::LoadSceneData()
{
	return S_OK;
}

void CLogoScene::Tick(const _double& _dTimeDelta)
{
}

void CLogoScene::LateTick(const _double& _dTimeDelta)
{
}

void CLogoScene::CollisionTick(const _double& _dTimeDelta)
{
}


END