#include "ClientDefines.h"
#include "CProtoMaker.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "CRooms.h"

HRESULT CProtoMaker::CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_ACTOR_MAINCAMERA, CreateConstructorToNative<CMainCamera>(_spDevice, LAYER_CAM, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_ROOM, CreateConstructorToNative<CRooms>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	
	return S_OK;
}
