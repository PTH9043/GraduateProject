#include "ClientDefines.h"
#include "CLogoScene.h"
#include "UGameInstance.h"
#include "CMainScene.h"

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
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
//	if (true == spGameInstance->IsNetworkResourceRecvSuccess())
	{
		spGameInstance->RegisterScene(CreateConstructorNative<CMainScene>(GetDevice()));
	}
}

void CLogoScene::LateTick(const _double& _dTimeDelta)
{
}

END