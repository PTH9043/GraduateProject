#include "ClientDefines.h"
#include "CNetworkClientController.h"
#include "CWarriorPlayer.h"
#include "CMainCamera.h"
#include "UGameInstance.h"
#include "UTransform.h"

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
				InsertNetworkActorContainer(CharInitData.first, spWarriorPlayer);
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

				spGameInstance->AddCollisionPawnList(spWarriorPlayer);
				InsertNetworkActorContainer(CharInitData.first, spWarriorPlayer);
			}
		}
		break;
		}
	}
	__super::MakeActors(_actorContainer);
#endif
}

void CNetworkClientController::CreateNetworkActor(_int _NetworkID, const NETWORKRECEIVEINITDATA& _networkInitData)
{
#ifdef _ENABLE_PROTOBUFF
	switch (_networkInitData.iType)
	{
	case TAG_CHAR::TAG_OTHERPLAYER:
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_NETWORKWARRIORANIMCONTROLLER, true };
			SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_WARRIORPLAYER, { &CharDesc }));

			spGameInstance->AddCollisionPawnList(spWarriorPlayer);
			InsertNetworkActorContainer(_NetworkID, spWarriorPlayer);
		}
	}
	break;
	}
#endif
}

void CNetworkClientController::NativePacket()
{
	__super::NativePacket();
}

void CNetworkClientController::ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead)
{
#ifdef _ENABLE_PROTOBUFF
	switch (_PacketHead.PacketType)
	{
		case TAG_SC::TAG_SC_CONNECTSUCCESS:
		{
			SC_CONNECTSUCCESS scConnectSuccess;
			scConnectSuccess.ParseFromArray(_pPacket, _PacketHead.PacketSize);
			{
				NETWORKRECEIVEINITDATA networkRecvInitData(scConnectSuccess.id(),
					scConnectSuccess.cellindex(), scConnectSuccess.type());
				// newwork Init Data 추가
				AddNetworkInitData(networkRecvInitData.iNetworkID, networkRecvInitData);
			}
			OutputDebugString(_wstring::to_string(scConnectSuccess.id()));
			// 로그인에 성공했다는 패킷 보내기
			CS_LOGIN csLogin;
			PROTOFUNC::MakeCsLogin(OUT & csLogin, GetNetworkOwnerID());
			SendProtoData(csLogin, TAG_CS::TAG_CS_LOGIN);
		}
		break; 
		case TAG_SC::TAG_SC_OTHERCLIENTLOGIN:
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
					CreateNetworkActor(networkRecvInitData.iNetworkID, networkRecvInitData);
					break;
				}
			}
		}
		break;
		case TAG_SC::TAG_SC_PLAYERSTATE:
		{

			SC_PLAYERSTATE scPlayerState;
			scPlayerState.ParseFromArray(_pPacket, _PacketHead.PacketSize);

			//// 해당하는 ID에 데이터 전달
			InsertProcessedDataInQuery(UProcessedData(scPlayerState.id(), scPlayerState, TAG_SC_PLAYERSTATE));
		}
		break;
	}
#endif
}

void CNetworkClientController::Free()
{
}
