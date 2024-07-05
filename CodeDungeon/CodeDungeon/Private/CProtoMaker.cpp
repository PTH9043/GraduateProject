#include "ClientDefines.h"
#include "CProtoMaker.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "CRooms.h"
#include "UAnimModel.h"
#include "CUserWarriorAnimController.h"
#include "CNetworkWarriorPlayerController.h"
#include "CMummyAnimController.h"
#include "CSarcophagusAnimController.h"
#include "CItemChestAnimController.h"
#include "UAnimModel.h"
#include "CWarriorPlayer.h"
#include "CMummy.h"
#include "CTorch.h"
#include "UTexGroup.h"
#include "CSword.h"
#include "CSarcophagus.h"
#include "CIronBars.h"
#include "CItemChest.h"

HRESULT CProtoMaker::CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_ACTOR_MAINCAMERA, CreateConstructorToNative<CMainCamera>(_spDevice, LAYER_CAM, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_ROOM, CreateConstructorToNative<CRooms>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_TORCH, CreateConstructorToNative<CTorch>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_IRONBARS, CreateConstructorToNative<CIronBars>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_WARRIORPLAYER, CreateConstructorToNative<CWarriorPlayer>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_MUMMY, CreateConstructorToNative<CMummy>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_SARCOPHAGUSLYING, CreateConstructorToNative<CSarcophagus>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_SARCOPHAGUSSTANDING, CreateConstructorToNative<CSarcophagus>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_LONGSWORD, CreateConstructorToNative< CSword>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_CHEST, CreateConstructorToNative< CItemChest>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));

	_spGameInstance->AddPrototype(PROTO_RES_PARTICLETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Particle", true));

	_spGameInstance->AddPrototype(PROTO_RES_ANIMPARTICLETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\AnimParticle", true));

	_spGameInstance->AddPrototype(PROTO_RES_FIRECOLORTEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Fire\\Color", true));

	_spGameInstance->AddPrototype(PROTO_RES_TRAILTEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Trail", true));
	
	_spGameInstance->AddPrototype(PROTO_RES_GUARDTEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Guard", true));

	_spGameInstance->AddPrototype(PROTO_RES_FIRENOISETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Fire\\Noise", true));

	_spGameInstance->AddPrototype(PROTO_RES_FIREALPHATEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Fire\\Alpha", true));


	_spGameInstance->AddPrototype(PROTO_COMP_USERWARRIORANIMCONTROLLER, CreateConstructorToNative<CUserWarriorAnimController>(_spDevice));
	_spGameInstance->AddPrototype(PROTO_COMP_NETWORKWARRIORANIMCONTROLLER, CreateConstructorNative<CNetworkWarriorPlayerController>(_spDevice));
	_spGameInstance->AddPrototype(PROTO_COMP_MUMMYANIMCONTROLLER, CreateConstructorToNative<CMummyAnimController>(_spDevice));
	_spGameInstance->AddPrototype(PROTO_COMP_SARCOPHAGUSANIMCONTROLLER, CreateConstructorToNative<CSarcophagusAnimController>(_spDevice));
	_spGameInstance->AddPrototype(PROTO_COMP_CHESTANIMCONTROLLER, CreateConstructorToNative<CItemChestAnimController>(_spDevice));
	return S_OK;
}

HRESULT CProtoMaker::CreateMainSceneProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_float4x4 Matrix = _float4x4::CreateScale(1.0f) /** _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180.f))*/;

	Matrix = _float4x4::CreateScale(10.f);
	_spGameInstance->AddPrototype(PROTO_RES_FEMAILPLAYERANIMMODEL,CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(
		_spDevice, L"..\\..\\Resource\\AnimModel\\Player\\Convert\\F_Human_FBX.bin", Matrix));

	Matrix = _float4x4::CreateScale(0.1f);
	_spGameInstance->AddPrototype(PROTO_RES_MUMMYANIMMODEL, CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(
		_spDevice, L"..\\..\\Resource\\AnimModel\\Mummy\\Convert\\Mummy_DEMO_1_FBX.bin", Matrix));

	Matrix = _float4x4::CreateScale(0.1f);
	_spGameInstance->AddPrototype(PROTO_RES_SARCOPHAGUSLYINGANIMMODEL, CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(
		_spDevice, L"..\\..\\Resource\\AnimModel\\Sarcophagus\\Convert\\SarcophagusLaying_FBX.bin", Matrix));

	Matrix = _float4x4::CreateScale(0.1f);
	_spGameInstance->AddPrototype(PROTO_RES_SARCOPHAGUSSTANDINGANIMMODEL, CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(
		_spDevice, L"..\\..\\Resource\\AnimModel\\Sarcophagus\\Convert\\SarcophagusStanding_FBX.bin", Matrix));


	_spGameInstance->AddPrototype(PROTO_RES_LONGSWORDMODEL, CLONETYPE::CLONE_STATIC, CreateConstructorNative<UModel>(
		_spDevice, L"..\\..\\Resource\\Model\\Item\\Equip\\Sword\\Convert\\sword_FBX.bin"));

	Matrix = _float4x4::CreateScale(100.0f);
	_spGameInstance->AddPrototype(PROTO_RES_CHESTANIMMODEL, CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(
		_spDevice, L"..\\..\\Resource\\AnimModel\\Chest\\Convert\\Chest_FBX.bin", Matrix));

	return S_OK;
}
