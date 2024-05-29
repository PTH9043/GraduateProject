#include "ClientDefines.h"
#include "CProtoMaker.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "CRooms.h"
#include "UAnimModel.h"
#include "CWarriorAnimController.h"
#include "CMummyAnimController.h"
#include "UAnimModel.h"
#include "CWarriorPlayer.h"
#include "CMummy.h"
#include "CTorch.h"
#include "UTexGroup.h"


HRESULT CProtoMaker::CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_ACTOR_MAINCAMERA, CreateConstructorToNative<CMainCamera>(_spDevice, LAYER_CAM, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_ROOM, CreateConstructorToNative<CRooms>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_TORCH, CreateConstructorToNative<CTorch>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_WARRIORPLAYER, CreateConstructorToNative<CWarriorPlayer>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));
	_spGameInstance->AddPrototype(PROTO_ACTOR_MUMMY, CreateConstructorToNative<CMummy>(_spDevice, LAYER_DEFAULT, CLONETYPE::CLONE_STATIC));

	_spGameInstance->AddPrototype(PROTO_RES_PARTICLETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Particle", true));

	_spGameInstance->AddPrototype(PROTO_RES_ANIMPARTICLETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\AnimParticle", true));

	_spGameInstance->AddPrototype(PROTO_RES_FIRECOLORTEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Fire\\Color", true));

	_spGameInstance->AddPrototype(PROTO_RES_FIRENOISETEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Fire\\Noise", true));

	_spGameInstance->AddPrototype(PROTO_RES_FIREALPHATEXTUREGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Fire\\Alpha", true));
	return S_OK;
}

HRESULT CProtoMaker::CreateMainSceneProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_COMP_WARRIORANIMCONTROLLER, CreateConstructorToNative<CWarriorAnimController>(_spDevice));
	_spGameInstance->AddPrototype(PROTO_COMP_MUMMYANIMCONTROLLER, CreateConstructorToNative<CMummyAnimController>(_spDevice));

	_float4x4 Matrix = _float4x4::CreateScale(0.1f) /** _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180.f))*/;

	Matrix = _float4x4::CreateScale(10.f);
	_spGameInstance->AddPrototype(PROTO_RES_FEMAILPLAYERANIMMODEL,CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(
		_spDevice, L"..\\..\\Resource\\AnimModel\\Player\\Convert\\F_Human_FBX.bin", Matrix));

	Matrix = _float4x4::CreateScale(0.1f);
	_spGameInstance->AddPrototype(PROTO_RES_MUMMYANIMMODEL, CLONETYPE::CLONE_STATIC, CreateConstructorNative<UAnimModel>(
		_spDevice, L"..\\..\\Resource\\AnimModel\\Mummy\\Convert\\Mummy_DEMO_1_FBX.bin", Matrix));


	
	return S_OK;
}
