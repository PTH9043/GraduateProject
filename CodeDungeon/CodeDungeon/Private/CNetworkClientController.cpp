#include "ClientDefines.h"
#include "CNetworkClientController.h"
#include "CWarriorPlayer.h"
#include "CMainCamera.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "CNetworkQueryProcessor.h"
#include "UMethod.h"
#include "CMummy.h"
#include "CSarcophagus.h"
#include "CMap.h"
#include "CMob.h"
#include "CItemChest.h"

CNetworkClientController::CNetworkClientController()
{
}

HRESULT CNetworkClientController::NativeConstruct(const _string& _strIPAddress, const _int _PortNumber)
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(_strIPAddress, _PortNumber), E_FAIL);
	return S_OK;
}

void CNetworkClientController::MakeActors(const VECTOR<SHPTR<UActor>>& _actorContainer)
{
#ifdef _ENABLE_PROTOBUFF

	CreateServerMobData();

	for (auto& CharInitData : GetNetworkInitDataContainer())
	{
		switch (CharInitData.second.iType)
		{
		case TAG_CHAR::TAG_MAINPLAYER:
		{
			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
			// Main Camera Load 
			{
				CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_USERWARRIORANIMCONTROLLER };
				CWarriorPlayer::PLAYERDESC PlayerDesc{ std::static_pointer_cast<UCamera>(_actorContainer[MAINCAMERA_ACTORS_ID])};
				SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_WARRIORPLAYER, { &CharDesc, &PlayerDesc }));

				spGameInstance->RegisterCurrentPlayer(spWarriorPlayer);
				AddCreatedNetworkActor(CharInitData.first, spWarriorPlayer);
				spGameInstance->RegisterCurrentPlayer(spWarriorPlayer);
				spGameInstance->AddPlayerContainer(spWarriorPlayer);
			}
		}
		break;
		case TAG_CHAR::TAG_OTHERPLAYER:
		{
			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
			// Main Camera Load 
			{
				CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_NETWORKWARRIORANIMCONTROLLER, true };
				SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_WARRIORPLAYER, { &CharDesc }));

				spGameInstance->RegisterCurrentPlayer(spWarriorPlayer);
				AddCreatedNetworkActor(CharInitData.first, spWarriorPlayer);
				spGameInstance->AddCollisionPawnList(spWarriorPlayer);
				spGameInstance->AddPlayerContainer(spWarriorPlayer);
			}
		}
		break;
		}
	}
	__super::MakeActors(_actorContainer);
#endif
}

void CNetworkClientController::NativePacket()
{
	__super::NativePacket();
}

void CNetworkClientController::ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead)
{
#ifdef _ENABLE_PROTOBUFF
	if (!strcmp(_pPacket, ""))
		return;

	if (_PacketHead.PacketSize <= 0)
		return;

	switch (_PacketHead.PacketType)
	{
		case TAG_SC::TAG_SC_CONNECTSUCCESS:
		{
			ConnectSuccessState(_pPacket, _PacketHead);
		}
		break; 
		case TAG_SC::TAG_SC_OTHERCLIENTLOGIN:
		{
			OtherClientLoginState(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_PLAYERSTATE:
		{
			PlayerAnimState(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_SELFPLAYERMOVE:
		{
			SelfPlayerMoveState(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_MONSTERSTATE:
		{
			MonsterState(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_MONSTERFIND:
		{
			FindMonster(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_DEAD:
		{
			Dead(_pPacket, _PacketHead);
		}
		break;
	}
#endif
}

#ifdef _ENABLE_PROTOBUFF

void CNetworkClientController::CreateServerMobData()
{
	VECTOR<MOBSERVERDATA> MobServerData;
	std::ifstream read{ L"..\\..\\Resource\\ServerLayout\\NetworkMobData.bin", std::ios::binary};
	assert(false == read.fail());

	size_t MobDatsSize = 0;
	read.read((_char*)&MobDatsSize, sizeof(size_t));
	MobServerData.reserve(MobDatsSize);
	for (_int i = 0; i < MobDatsSize; ++i)
	{
		MOBSERVERDATA mobServerData;
		read.read((_char*)&mobServerData, sizeof(MOBSERVERDATA));
		MobServerData.emplace_back(mobServerData);
	}

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	for (auto& iter : MobServerData)
	{
		switch (iter.iMobType)
		{
		case TAG_CHAR::TAG_CHEST:
		{
			CItemChest::CHARACTERDESC chestDesc{ PROTO_RES_CHESTANIMMODEL, PROTO_COMP_CHESTANIMCONTROLLER };;
			SHPTR<CItemChest> Chest = std::static_pointer_cast<CItemChest>(spGameInstance->CloneActorAdd(PROTO_ACTOR_CHEST, { &chestDesc, &iter }));
			AddCreatedNetworkActor(iter.iMobID, Chest);
			spGameInstance->AddCollisionPawnList(Chest);
		}
			break;
		case TAG_CHAR::TAG_SARCOPHAGUS_LAYING:
		{
			CSarcophagus::CHARACTERDESC SarcDesc{ PROTO_RES_SARCOPHAGUSLYINGANIMMODEL, PROTO_COMP_SARCOPHAGUSANIMCONTROLLER };
			SHPTR<CSarcophagus> Saracophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_SARCOPHAGUSLYING, { &SarcDesc, &iter }));
			AddCreatedNetworkActor(iter.iMobID, Saracophagus);
			Saracophagus->SetSarcophagusType(CSarcophagus::SARCOTYPE::TYPE_LYING);
		//	spGameInstance->AddCollisionPawnList(Saracophagus);
		}
			break;
		case TAG_CHAR::TAG_SARCOPHAGUS_STANDING:
		{
			CSarcophagus::CHARACTERDESC SarcDesc{ PROTO_RES_SARCOPHAGUSSTANDINGANIMMODEL, PROTO_COMP_SARCOPHAGUSANIMCONTROLLER };
			SHPTR<CSarcophagus> Saracophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_SARCOPHAGUSLYING, { &SarcDesc, &iter }));
			AddCreatedNetworkActor(iter.iMobID, Saracophagus);
			Saracophagus->SetSarcophagusType(CSarcophagus::SARCOTYPE::TYPE_STANDING);
		//	spGameInstance->AddCollisionPawnList(Saracophagus);
		}
			break;
		case TAG_CHAR::TAG_MUMMY_LAYING:
		{
			CMummy::CHARACTERDESC MummyDesc{ PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_MUMMYANIMCONTROLLER };;
			SHPTR<CMummy> Mummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MUMMY, { &MummyDesc, &iter }));
			AddCreatedNetworkActor(iter.iMobID, Mummy);
			Mummy->SetMummyType(CMummy::TYPE_LYING);
			spGameInstance->AddCollisionPawnList(Mummy);
		}
			break;
		case TAG_CHAR::TAG_MUMMY_STANDING:
		{
			CMummy::CHARACTERDESC MummyDesc{ PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_MUMMYANIMCONTROLLER };;
			SHPTR<CMummy> Mummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MUMMY, { &MummyDesc, &iter }));
			AddCreatedNetworkActor(iter.iMobID, Mummy);
			Mummy->SetMummyType(CMummy::TYPE_STANDING);
			spGameInstance->AddCollisionPawnList(Mummy);
		}
			break;
		}
	}
}

void CNetworkClientController::ConnectSuccessState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_CONNECTSUCCESS scConnectSuccess;
	scConnectSuccess.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	{
		NETWORKRECEIVEINITDATA networkRecvInitData(scConnectSuccess.id(),
			scConnectSuccess.cellindex(), scConnectSuccess.type());
		// newwork Init Data 추가
		AddNetworkInitData(networkRecvInitData.iNetworkID, networkRecvInitData);

		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		SetNetworkOwnerID(scConnectSuccess.id());
	}
	// 로그인에 성공했다는 패킷 보내기
	CS_LOGIN csLogin;
	PROTOFUNC::MakeCsLogin(OUT & csLogin, GetNetworkOwnerID());
	SendProtoData(csLogin, TAG_CS::TAG_CS_LOGIN);
}

void CNetworkClientController::OtherClientLoginState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_OTHERCLIENTLOGIN csOtherClientLogin;
	csOtherClientLogin.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	{
		NETWORKRECEIVEINITDATA networkRecvInitData(csOtherClientLogin.id(),
			csOtherClientLogin.cellindex(), csOtherClientLogin.type());
		switch (GetSceneID())
		{
		case SCENE_LOGO:
			AddNetworkInitData(networkRecvInitData.iNetworkID, networkRecvInitData);
			break;
		case SCENE_STAGE1:
			InsertNetworkInitDataInQuery(networkRecvInitData);
			break;
		}
	}
}

void CNetworkClientController::PlayerAnimState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	CHARSTATE charMove;
	charMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertNetworkProcessInQuery(std::move(UProcessedData(charMove.id(), charMove,
		TAG_SC_PLAYERSTATE, _PacketHead.PacketSize)));
}

void CNetworkClientController::SelfPlayerMoveState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_SEEPLAYERMOVE selfPlayerMove;
	selfPlayerMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertNetworkProcessInQuery(std::move(UProcessedData(selfPlayerMove.id(), selfPlayerMove,
		TAG_SC_SELFPLAYERMOVE, _PacketHead.PacketSize)));
}

void CNetworkClientController::MonsterState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	CHARSTATE scMonsterState;
	scMonsterState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertNetworkProcessInQuery(UProcessedData(scMonsterState.id(), scMonsterState,
		TAG_SC_MONSTERSTATE, _PacketHead.PacketSize));
}

void CNetworkClientController::FindMonster(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_MONSTERFIND scFindMonster;
	scFindMonster.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertNetworkProcessInQuery(UProcessedData(scFindMonster.id(), scFindMonster,
		TAG_SC_MONSTERFIND, _PacketHead.PacketSize));
}

void CNetworkClientController::Dead(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_DEAD scCharHp;
	scCharHp.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertNetworkProcessInQuery(UProcessedData(scCharHp.id(), scCharHp,
		TAG_SC_DEAD, _PacketHead.PacketSize));
}

#endif

void CNetworkClientController::Free()
{
}
