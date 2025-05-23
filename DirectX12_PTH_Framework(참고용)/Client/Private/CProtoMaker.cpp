#include "ClientDefine.h"
#include "CProtoMaker.h"
#include "UGameInstance.h"

#include "UDevice.h"
#include "UCommand.h"

#include "CMainCamera.h"
#include "UVIBufferTerrain.h"
#include "UTexGroup.h"
#include "URawData.h"

#include "CPlayer.h"
#include "UModel.h"
#include "CTree.h"
#include "CWater.h"
#include "CPlayerGun.h"
#include "CAimingPoint.h"
#include "CMonster.h"
#include "CBullet.h"
#include "CBuilding.h"

HRESULT CProtoMaker::CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_ACTOR_MAINCAMERA, CreateConstructorToNative<CMainCamera>(_spDevice, LAYTER_CAM, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_PLAYER, CreateConstructorToNative<CPlayer>(_spDevice, LAYER_PLAYER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_TREE, CreateConstructorToNative<CTree>(_spDevice, LAYER_PLAYER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_WATER, CreateConstructorToNative<CWater>(_spDevice, LAYER_WATER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_PLAYERGUN, CreateConstructorToNative<CPlayerGun>(_spDevice, LAYER_ITEM, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_AIMPOINT_UI, CreateConstructorToNative<CAimingPoint>(_spDevice, LAYER_UI, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_MONSTER, CreateConstructorToNative<CMonster>(_spDevice, LAYER_MONSTER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_BULLET, CreateConstructorToNative<CBullet>(_spDevice, LAYER_BULLET, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_BUILDING, CreateConstructorToNative<CBuilding>(_spDevice, LAYER_BUILD, CLONETYPE::CLONE_ONCE));

	_spGameInstance->FontCreate(FONT_NAMUSQAURE, L"..\\..\\Resource\\Font\\NanumSquare.spritefont");

	SHPTR<UVIBufferTerrain> pTerrain = CreateConstructorNative<UVIBufferTerrain>(_spDevice,
		L"..\\..\\Resource\\Terrain\\HeightMap\\Height.bmp", 10.f,
		D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	_spGameInstance->AddPrototype(PROTO_RES_VIBUFFERTERRAIN, CLONETYPE::CLONE_STATIC, pTerrain);

	_spGameInstance->AddPrototype(PROTO_RES_TERRAINGROUP, CLONETYPE::CLONE_STATIC,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Terrain\\", true, true));

	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Eviornmemt\\Build\\Convert\\Buld01_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative<UModel>(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_BUILDINGMODEL_01, CLONETYPE::CLONE_STATIC, spModel);
	}
	return S_OK;
}

HRESULT CProtoMaker::LoadStageScene1(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{

	return S_OK;
}

HRESULT CProtoMaker::LoadStageScene2(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{	
	// 	L"..\\..\\Resource\\Terrain\\HeightMap\\Height.bmp", 1.f, 
	//
	//SHPTR<UVIBufferTerrain> pTerrain = CreateConstructorNative<UVIBufferTerrain>(_spDevice,
	//	L"..\\..\\Resource\\Terrain\\HeightMap\\Height.bmp", 10.f,
	//	D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	//_spGameInstance->AddPrototype(PROTO_RES_VIBUFFERTERRAIN, CLONETYPE::CLONE_ONCE, pTerrain);

	//_spGameInstance->AddPrototype(PROTO_RES_TERRAINGROUP, CLONETYPE::CLONE_ONCE,
	//	CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Terrain\\", true, true));
	return S_OK;
}
