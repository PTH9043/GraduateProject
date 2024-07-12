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

CNetworkClientController::CNetworkClientController()
{
}

HRESULT CNetworkClientController::NativeConstruct(const _string& _strIPAddress, const _int _PortNumber)
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(_strIPAddress, _PortNumber), E_FAIL);
	return S_OK;
}

void CNetworkClientController::MakeActors(const VECTOR<SHPTR<UActor>>& _actorContainer, void* _pMapData)
{
#ifdef _ENABLE_PROTOBUFF
	CMap* pMap = static_cast<CMap*>(_pMapData);

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
			}
		}
		break;
		case TAG_CHAR::TAG_OTHERPLAYER:
		{
			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
			// Main Camera Load 
			{
				CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_NETWORKWARRIORANIMCONTROLLER, true };
				CWarriorPlayer::PLAYERDESC PlayerDesc{ std::static_pointer_cast<UCamera>(_actorContainer[MAINCAMERA_ACTORS_ID]) };
				SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_WARRIORPLAYER, { &CharDesc, &PlayerDesc }));

				spGameInstance->RegisterCurrentPlayer(spWarriorPlayer);
				AddCreatedNetworkActor(CharInitData.first, spWarriorPlayer);
			}
		}
		break;
		case TAG_CHAR::TAG_MUMMY:
		{

		}
		break;
		case TAG_CHAR::TAG_SARCOPHAGUS_STANDING:
		case TAG_CHAR::TAG_SARCOPHAGUS_LAYING:
		{
		}
		break;
		}
	}
	__super::MakeActors(_actorContainer, _pMapData);
#endif
}

void CNetworkClientController::NativePacket()
{
	__super::NativePacket();
}

void CNetworkClientController::ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead)
{
#ifdef _ENABLE_PROTOBUFF
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
		case TAG_SC::TAG_SC_START_INFORMATION_SUCCESS:
		{
			StartNetworkInfoSucess(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_MONSTERRESOURCEDATA:
		{
			MonsterResourceDataState(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_PLAYERSTATE:
		{
			PlayerAnimState(_pPacket, _PacketHead);
		}
		break;
		case TAG_SC::TAG_SC_CHARMOVE:
		{
			CharMoveState(_pPacket, _PacketHead);
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
		case TAG_SC::TAG_SC_MONSTERSTATEHAVEMOVE:
		{
			MonsterStateHaveMove(_pPacket, _PacketHead);
		}
		break;
	}
#endif
}

#ifdef _ENABLE_PROTOBUFF

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

void CNetworkClientController::StartNetworkInfoSucess(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_START_INFORMATION_SUCCESS scStartInfoNetworkSuccess;
	scStartInfoNetworkSuccess.ParseFromArray(_pPacket, _PacketHead.PacketSize);

	if (scStartInfoNetworkSuccess.id() == GetNetworkOwnerID())
	{
		EnableNetworkResourceRecvSuccess();
		SetMakeMonsterNum(scStartInfoNetworkSuccess.monsternum());
	}
}

void CNetworkClientController::MonsterResourceDataState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_MONSTERRESOURCEDATA scMonsterResourceData;
	scMonsterResourceData.ParseFromArray(_pPacket, _PacketHead.PacketSize);

	_float3 vPos{ scMonsterResourceData.posx(),scMonsterResourceData.posy(),scMonsterResourceData.posz() };
	{
		NETWORKRECEIVEINITDATA networkRecvInitData(scMonsterResourceData.id(),
			scMonsterResourceData.cellindex(), scMonsterResourceData.type(), scMonsterResourceData.animindex(), vPos);
		InsertNetworkInitDataInQuery(networkRecvInitData);
	}
}

void CNetworkClientController::PlayerAnimState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	PLAYERSTATE PlayerState;
	PlayerState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertNetworkProcessInQuery(std::move(UProcessedData(PlayerState.id(), PlayerState, TAG_SC_PLAYERSTATE,
		_PacketHead.PacketSize)));
}

void CNetworkClientController::CharMoveState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	CHARMOVE charMove;
	charMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertNetworkProcessInQuery(std::move(UProcessedData(charMove.id(), charMove,
		TAG_SC_CHARMOVE, _PacketHead.PacketSize)));
}

void CNetworkClientController::SelfPlayerMoveState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SELFPLAYERMOVE selfPlayerMove;
	selfPlayerMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	//// 해당하는 ID에 데이터 전달
	InsertNetworkProcessInQuery(std::move(UProcessedData(selfPlayerMove.id(), selfPlayerMove,
		TAG_SC_SELFPLAYERMOVE, _PacketHead.PacketSize)));
}

void CNetworkClientController::MonsterState(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_MONSTERSTATE scMonsterState;
	scMonsterState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertNetworkProcessInQuery(UProcessedData(scMonsterState.id(), scMonsterState,
		TAG_SC_MONSTERSTATE, _PacketHead.PacketSize));
}

void CNetworkClientController::MonsterStateHaveMove(_char* _pPacket, const PACKETHEAD& _PacketHead)
{
	SC_MONSTERSTATEHAVEPOS scMonsterState;
	scMonsterState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
	InsertNetworkProcessInQuery(UProcessedData(scMonsterState.id(), scMonsterState,
		TAG_SC_MONSTERSTATEHAVEMOVE, _PacketHead.PacketSize));
}

#endif

void CNetworkClientController::Free()
{
}
