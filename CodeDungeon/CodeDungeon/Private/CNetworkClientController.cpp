#include "ClientDefines.h"
#include "CNetworkClientController.h"
#include "CWarriorPlayer.h"
#include "CMainCamera.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CMummy.h"
#include "CSarcophagus.h"
#include "CMap.h"
#include "CMob.h"
#include "CItemChest.h"
#include "CAnubis.h"
#include "CHarlequinn.h"
#include "CMinotaur.h"
#include "CMimic.h"
#include "CIronBars.h"
#include "CStatue.h"
#include "CCoreAnubis.h"
#include "CCoreHarlequinn.h"
#include "CCoreMinotaur.h"
#include "UModel.h"
#include "CProtoMaker.h"

CNetworkClientController::CNetworkClientController() 
{
}

HRESULT CNetworkClientController::NativeConstruct(const _string& _strIPAddress, const _int _PortNumber)
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(_strIPAddress, _PortNumber), E_FAIL);
	return S_OK;
}

void CNetworkClientController::MakeActorsInit(const VECTOR<SHPTR<UBase>>& _actorContainer)
{
	{
		std::mutex Lock;
		std::lock_guard<std::mutex> LL(Lock);
		DisableNetworkTickRunning();

		CMap* pMap = (CMap*)(_actorContainer[1].get());

		CreateServerMobData(pMap);
		CreateStaticObjData(pMap);

		const NETWORKINITDATACONTAINER& InitNetworkDataContainer = GetNetworkInitDataContainer();

		for (auto& CharInitData : InitNetworkDataContainer)
		{
			switch (CharInitData.second.iType)
			{
			case TAG_CHAR::TAG_MAINPLAYER:
			{
				SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
				// Main Camera Load 
				{
					CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_USERWARRIORANIMCONTROLLER };
					CWarriorPlayer::PLAYERDESC PlayerDesc{ std::static_pointer_cast<UCamera>(_actorContainer[MAINCAMERA_ACTORS_ID]) };
					SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
						PROTO_ACTOR_WARRIORPLAYER, { &CharDesc, &PlayerDesc }));
					spGameInstance->RegisterCurrentPlayer(spWarriorPlayer);
					InsertNetworkActorInContainer(CharInitData.first, spWarriorPlayer);

				}
			}
			break;
			case TAG_CHAR::TAG_OTHERPLAYER:
			{
				SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
				{
					CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_NETWORKWARRIORANIMCONTROLLER, true };
					SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
						PROTO_ACTOR_WARRIORPLAYER, { &CharDesc }));
					spGameInstance->AddPlayerInContainer(spWarriorPlayer);
					InsertNetworkActorInContainer(CharInitData.first, spWarriorPlayer);
				}
			}
			break;
			}
		}
		__super::MakeActorsInit(_actorContainer);
	}
	ThreadMiliRelax(100);
}

void CNetworkClientController::MakeActorsTick()
{
	for (auto& CharInitData : GetNetworkInitDataContainer())
	{
		switch (CharInitData.second.iType)
		{
			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
			{
				CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_NETWORKWARRIORANIMCONTROLLER, true };
				SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_WARRIORPLAYER, { &CharDesc }));
				spGameInstance->AddPlayerInContainer(spWarriorPlayer);
				InsertNetworkActorInContainer(CharInitData.first, spWarriorPlayer);
			}
		}
	}
	__super::MakeActorsTick();
}

void CNetworkClientController::NativePacket()
{
	__super::NativePacket();
}

void CNetworkClientController::ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead)
{
	size_t value = GetNetworkActorContainer().size();

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
	case TAG_SC::TAG_SC_DAMAGED:
	{
		CharDamaged(_pPacket, _PacketHead);
	}
	break;
	case TAG_SC::TAG_SC_MONSTERSTATE:
	{
		MonsterState(_pPacket, _PacketHead);
	}
	break;
	case TAG_SC::TAG_SC_MONSTERFIND:
	{
		MonsterFind(_pPacket, _PacketHead);
	}
	break;
	case TAG_SC::TAG_SC_STATICOBJFIND:
	{
		StaticObjFind(_pPacket, _PacketHead);
	}
	break;
	case TAG_SC::TAG_SC_HARLEQUINTHROWING:
	{
		HarlequinThrowing(_pPacket, _PacketHead);
	}
	break;
	case TAG_SC::TAG_SC_SAVEPOINTENABLE:
	{
		SavePointEnabe(_pPacket, _PacketHead);
	}
	break;
	case TAG_SC::TAG_SC_PLAYERGETUP:
	{
		PlayerGetup(_pPacket, _PacketHead);
	}
	break;
	case TAG_SC::TAG_SC_ENDING:
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		SHPTR<CWarriorPlayer> spPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->GetCurrPlayer());
		spPlayer->SetIfPlayerEndEnable();
	}
	break;
	}
}

#ifdef _ENABLE_PROTOBUFF

void CNetworkClientController::CreateServerMobData(CMap* _pMap)
{

	CMobServerLayoutLoader Loader{ "..\\..\\Resource\\ServerLayout\\NetworkMobData.bin"};

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	UNORMAP<_int, CSarcophagus*> SaracophagusContainer;
	UNORMAP<_int, SHPTR<CMummy>> MuumyContainer;
	SHPTR<MOBSCONTAINER> MobContainer = Create<MOBSCONTAINER>();

	for (auto& iter : Loader.GetMobData())
	{
		SHPTR<CMob> spMob;
		switch (iter.iMobType)
		{
		case TAG_CHAR::TAG_CHEST:
		{
			CItemChest::CHARACTERDESC chestDesc{ PROTO_RES_CHESTANIMMODEL, PROTO_COMP_CHESTANIMCONTROLLER };
			SHPTR<CItemChest> Chest = std::static_pointer_cast<CItemChest>(spGameInstance->CloneActorAdd(PROTO_ACTOR_CHEST, { &chestDesc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Chest);
			spMob = Chest;
		}
			break;
		case TAG_CHAR::TAG_SARCOPHAGUS_LAYING:
		{
			CSarcophagus::CHARACTERDESC SarcDesc{ PROTO_RES_SARCOPHAGUSLYINGANIMMODEL, PROTO_COMP_SARCOPHAGUSANIMCONTROLLER };
			SHPTR<CSarcophagus> Saracophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_SARCOPHAGUSLYING, { &SarcDesc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Saracophagus);
			Saracophagus->SetSarcophagusType(CSarcophagus::SARCOTYPE::TYPE_LYING);
			SaracophagusContainer.insert(MakePair(iter.iMobParentsID, Saracophagus.get()));
			spMob = Saracophagus;
		}
			break;
		case TAG_CHAR::TAG_SARCOPHAGUS_STANDING:
		{
			CSarcophagus::CHARACTERDESC SarcDesc{ PROTO_RES_SARCOPHAGUSSTANDINGANIMMODEL, PROTO_COMP_SARCOPHAGUSANIMCONTROLLER };
			SHPTR<CSarcophagus> Saracophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_SARCOPHAGUSSTANDING, { &SarcDesc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Saracophagus);
			Saracophagus->SetSarcophagusType(CSarcophagus::SARCOTYPE::TYPE_STANDING);
			SaracophagusContainer.insert(MakePair(iter.iMobParentsID, Saracophagus.get()));
			spMob = Saracophagus;
		}
			break;
		case TAG_CHAR::TAG_MUMMY_LAYING:
		{
			CMummy::CHARACTERDESC MummyDesc{ PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_NETWORKMUMMYANIMCONTROLLER };;
			SHPTR<CMummy> Mummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MUMMY, { &MummyDesc, &iter }));	
			InsertNetworkActorInContainer(iter.iMobID, Mummy);
			Mummy->SetMummyType(CMummy::TYPE_LYING);
			MuumyContainer.insert(MakePair(Mummy->GetNetworkID(), Mummy));
			spMob = Mummy;
		}
			break;
		case TAG_CHAR::TAG_MUMMY_STANDING:
		{
			CMummy::CHARACTERDESC MummyDesc{ PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_NETWORKMUMMYANIMCONTROLLER };;
			SHPTR<CMummy> Mummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MUMMY, { &MummyDesc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Mummy);
			Mummy->SetMummyType(CMummy::TYPE_STANDING);
			MuumyContainer.insert(MakePair(Mummy->GetNetworkID(), Mummy));
			spMob = Mummy;
		}
			break;
		case TAG_CHAR::TAG_ANUBIS:
		{
			CAnubis::CHARACTERDESC Desc{ PROTO_RES_ANUBISANIMMODEL, PROTO_COMP_NETWORKANUBISANIMCONTROLLER };;
			SHPTR<CAnubis> Anubis = std::static_pointer_cast<CAnubis>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ANUBIS, { &Desc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Anubis);
			spMob = Anubis;
		}
		break;
		case TAG_CHAR::TAG_MIMIC:
		{
			CAnubis::CHARACTERDESC Desc{ PROTO_RES_MIMICANIMMODEL, PROTO_COMP_NETWORKMIMICANIMCONTROLLER };;
			SHPTR<CMimic> Mimic = std::static_pointer_cast<CMimic>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MIMIC, { &Desc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Mimic);
			spMob = Mimic;
		}
		break;
		case TAG_CHAR::TAG_MINOTAUR:
		{
			CAnubis::CHARACTERDESC Desc{ PROTO_RES_MINOTAURANIMMODEL, PROTO_COMP_NETWORKMINOTAURANIMCONTROLLER };;
			SHPTR<CMinotaur> Minotaur = std::static_pointer_cast<CMinotaur>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MINOTAUR, { &Desc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Minotaur);
			spMob = Minotaur;
		}
		break;
		case TAG_CHAR::TAG_HARLEQUINN:
		{
			CAnubis::CHARACTERDESC Desc{ PROTO_RES_HARLEQUINNANIMMODEL, PROTO_COMP_NETWORKHARLEQUINNANIMCONTROLLER };;
			SHPTR<CHarlequinn> Harlequin = std::static_pointer_cast<CHarlequinn>(spGameInstance->CloneActorAdd(PROTO_ACTOR_HARLEQUINN, { &Desc, &iter }));
			InsertNetworkActorInContainer(iter.iMobID, Harlequin);
			spMob = Harlequin;
		}
		break;
		}

		(*MobContainer)[UMethod::ConvertSToW(iter.strRoomName)].push_back(spMob);
	}

	for (auto& iter : SaracophagusContainer)
	{
		const auto& MummyPair = MuumyContainer.find(iter.first);
		iter.second->SetOwnerMummy(MummyPair->second);
	}

	//const auto& findIter1 = (*MobContainer).find(L"Interior_Room_G");
	//for (auto& iter : findIter1->second)
	//{
	//	iter->SetDeadDissolveEnable(true);
	//	iter->SetDeathState(true);
	//}
	//const auto& findIter2 = (*MobContainer).find(L"Interior_Hallway_E");
	//for (auto& iter : findIter2->second)
	//{
	//	iter->SetDeadDissolveEnable(true);
	//	iter->SetDeathState(true);
	//}
	_pMap->SetMobContainer(MobContainer);
}

void CNetworkClientController::CreateStaticObjData(CMap* _pMap)
{
	CStaticObjServerLayoutLoader Loader{ "..\\..\\Resource\\ServerLayout\\NetworkStaticObjData.bin" };

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<STATICOBJCONTAINER> staticObjContainer = Create<STATICOBJCONTAINER>();

	for (auto& iter : Loader.GetStaticObjData())
	{
		SHPTR<CModelObjects> spModelObj;

		switch (iter.iObjType)
		{
		case TAG_STATUE:
		{
			CStatue::STATUEDESC StatueDesc;
			StatueDesc._Worldm = iter.mWorldMatrix;
			SHPTR<CStatue> Statue = std::static_pointer_cast<CStatue>(spGameInstance->CloneActorAdd(PROTO_ACTOR_STATUE, { &StatueDesc }));
			spGameInstance->AddCollisionPawnList(Statue);
			InsertNetworkActorInContainer(iter.iServerID, Statue);
			spModelObj = Statue;
		}
			break;
		case TAG_IRONBAR:
		{
			CIronBars::IRONBARSDESC BarsDesc;
			BarsDesc._Worldm = iter.mWorldMatrix;
			SHPTR<CIronBars> _IronBars = std::static_pointer_cast<CIronBars>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IRONBARS, { &BarsDesc }));
			spGameInstance->AddCollisionPawnList(_IronBars);
			InsertNetworkActorInContainer(iter.iServerID, _IronBars);
			spModelObj = _IronBars;
		}
		break;
		case TAG_COREANUBIS:
		{
			CCoreAnubis::COREANUBISNDESC CoreAnubisDesc;
			CoreAnubisDesc._Worldm = iter.mWorldMatrix;
			SHPTR<CCoreAnubis> CoreAnubis = std::static_pointer_cast<CCoreAnubis>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ANUBISCORE, { &CoreAnubisDesc }));
			spGameInstance->AddCollisionPawnList(CoreAnubis);
			InsertNetworkActorInContainer(iter.iServerID, CoreAnubis);
			CoreAnubis->GetModel()->SetModelName(L"AnubisCore");
			spModelObj = CoreAnubis;
		}
		break;
		case TAG_COREHARLEQUIN:
		{
			CCoreHarlequinn::COREHARLEQUINNDESC Desc;
			Desc._Worldm = iter.mWorldMatrix;
			SHPTR<CCoreHarlequinn> Core = std::static_pointer_cast<CCoreHarlequinn>(spGameInstance->CloneActorAdd(PROTO_ACTOR_HARLEQUINNCORE, { &Desc }));
			spGameInstance->AddCollisionPawnList(Core);
			InsertNetworkActorInContainer(iter.iServerID, Core);
			Core->GetModel()->SetModelName(L"HarlequinnCore");
			spModelObj = Core;
		}
		break;
		case TAG_COREMINOTAUR:
		{
			CCoreMinotaur::COREMINOTAURDESC Desc;
			Desc._Worldm = iter.mWorldMatrix;
			SHPTR<CCoreMinotaur> Core = std::static_pointer_cast<CCoreMinotaur>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MINOTAURCORE, { &Desc }));
			spGameInstance->AddCollisionPawnList(Core);
			InsertNetworkActorInContainer(iter.iServerID, Core);
			Core->GetModel()->SetModelName(L"MinotaurCore");
			spModelObj = Core;
		}
		break;
		}

		(*staticObjContainer)[UMethod::ConvertSToW(iter.strRoomName)].push_back(spModelObj);
	}
	_pMap->InsertStaticObjContainer(staticObjContainer);
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
		AddNetworkInitData(networkRecvInitData.iNetworkID, networkRecvInitData);
	}
}

void CNetworkClientController::PlayerAnimState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static CHARSTATE PlayerState;
	PlayerState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertProcessedDataInActor(UProcessedData(PlayerState.id(), PlayerState, TAG_SC_PLAYERSTATE,
		_PacketHead.PacketSize));
	PlayerState.Clear();
}

void CNetworkClientController::CharDamaged(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static SC_DAMAGED Damaged;
	Damaged.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertProcessedDataInActor(UProcessedData(Damaged.id(), Damaged, TAG_SC_DAMAGED,
		_PacketHead.PacketSize));
	Damaged.Clear();
}

void CNetworkClientController::SelfPlayerMoveState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_SEEPLAYERMOVE selfPlayerMove;
	selfPlayerMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertProcessedDataInActor(UProcessedData(selfPlayerMove.id(), selfPlayerMove,
		TAG_SC_SELFPLAYERMOVE, _PacketHead.PacketSize));
}

void CNetworkClientController::MonsterState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static MOBSTATE MobState;
	MobState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertProcessedDataInActor(UProcessedData(MobState.id(), MobState,
		TAG_SC_MONSTERSTATE, _PacketHead.PacketSize));
	MobState.Clear();
}

void CNetworkClientController::MonsterFind(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static SC_MONSTERFIND scMonsterFind;
	scMonsterFind.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertProcessedDataInActor(UProcessedData(scMonsterFind.id(), scMonsterFind,
		TAG_SC_MONSTERFIND, _PacketHead.PacketSize));
	scMonsterFind.Clear();
}
void CNetworkClientController::StaticObjFind(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static SC_STATICOBJFIND scStaticObjFind;
	scStaticObjFind.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertProcessedDataInActor(UProcessedData(scStaticObjFind.id(), scStaticObjFind,
		TAG_SC_STATICOBJFIND, _PacketHead.PacketSize));
	scStaticObjFind.Clear();
}

void CNetworkClientController::HarlequinThrowing(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static SC_HARLEQUINTHROWING scHarlequinThrowing;
	scHarlequinThrowing.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertProcessedDataInActor(UProcessedData(scHarlequinThrowing.id(), scHarlequinThrowing,
		TAG_SC_HARLEQUINTHROWING, _PacketHead.PacketSize));
	scHarlequinThrowing.Clear();
}

void CNetworkClientController::SavePointEnabe(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static SC_SAVEPOINTENABLE savePointEnable;
	savePointEnable.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertProcessedDataInActor(UProcessedData(savePointEnable.id(), savePointEnable,
		TAG_SC_SAVEPOINTENABLE, _PacketHead.PacketSize));
	savePointEnable.Clear();
}

void CNetworkClientController::PlayerGetup(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	static SC_PLAYERGETUP playerGetUp;
	playerGetUp.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertProcessedDataInActor(UProcessedData(playerGetUp.id(), playerGetUp,
		TAG_SC_PLAYERGETUP, _PacketHead.PacketSize));
	playerGetUp.Clear();
}

#endif

void CNetworkClientController::Free()
{
}
