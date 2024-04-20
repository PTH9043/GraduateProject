#include "ClientDefines.h"
#include "CMainScene.h"

BEGIN(Client)

CMainScene::CMainScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice)
{
}

CMainScene::CMainScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice)
{
}

void CMainScene::Free()
{
}

HRESULT CMainScene::LoadSceneData()
{
	return S_OK;
}

void CMainScene::Tick(const _double& _dTimeDelta)
{
}

void CMainScene::LateTick(const _double& _dTimeDelta)
{
}

END
