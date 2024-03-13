#include "ToolDefines.h"
#include "TProtoMaker.h"
#include "UGameInstance.h"

#include "UDevice.h"
#include "UCommand.h"
#include "TTestObject.h"
#include "TShowAnimModelObject.h"
#include "TShowModelObject.h"
#include "TMainCamera.h"
#include "TAnimControlModel.h"
#include "UTexGroup.h"

HRESULT TProtoMaker::CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance,
	CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(L"Proto_Actor_TestObject",CreateConstructorToNative<TTestObject>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_MAINCAMERA, CreateConstructorToNative<TMainCamera>(_spDevice, LAYER_CAM, CLONETYPE::CLONE_STATIC));

	_spGameInstance->AddPrototype(PROTO_ACTOR_SHOWMODELOBJECT, CreateConstructorToNative<TShowModelObject>(_spDevice, LAYER_SHOWMODELL, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_SHOWANIMMODELOBJECT, CreateConstructorToNative<TShowAnimModelObject>(_spDevice, LAYER_SHOWMODELL, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_ANIMCONTROLMODELOBJECT, CreateConstructorToNative<TAnimControlModel>(_spDevice, LAYER_SHOWMODELL, CLONETYPE::CLONE_STATIC));
	
	_spGameInstance->AddPrototype(PROTO_RES_PARTICLETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Particle", true));

	_spGameInstance->AddPrototype(PROTO_RES_ANIMPARTICLETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\AnimParticle", true));

	_spGameInstance->AddPrototype(PROTO_RES_FIRETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Fire", true));

	_spGameInstance->CreateAudioSystemToFolderNameAndRegister(SOUNDTYPE::SOUND_BACKGROUND, L"BackgroundSound");
	_spGameInstance->CreateAudioSystemToFolderNameAndRegister(SOUNDTYPE::SOUND_GAME, L"GameSound");
	return S_OK;
}
