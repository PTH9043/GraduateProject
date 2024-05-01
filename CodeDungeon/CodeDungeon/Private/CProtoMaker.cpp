#include "ClientDefines.h"
#include "CProtoMaker.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "CRooms.h"
#include "UAnimModel.h"
#include "CWarriorAnimController.h"
#include "UAnimModel.h"
#include "CWarriorPlayer.h"

HRESULT CProtoMaker::CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_ACTOR_MAINCAMERA, CreateConstructorToNative<CMainCamera>(_spDevice, LAYER_CAM, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_ROOM, CreateConstructorToNative<CRooms>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_WARRIORPLAYER, CreateConstructorToNative<CWarriorPlayer>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	return S_OK;
}

HRESULT CProtoMaker::CreateMainSceneProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_COMP_WARRIORANIMCONTROLLER, CreateConstructorToNative<CWarriorAnimController>(_spDevice));
	
	_spGameInstance->AddPrototype(PROTO_RES_FEMAILPLAYERANIMMODEL,CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(_spDevice, L"..\\..\\Resource\\AnimModel\\Player\\Convert\\FemalePlayer_FBX.bin"));
	return S_OK;
}
