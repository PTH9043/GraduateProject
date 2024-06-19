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

void CNetworkClientController::MakeActors()
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
				UCamera::CAMDESC tDesc;
				tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::PERSPECTIVE, _float3(0.f, 0.f, 0.f),
					_float3(0.f, 0.f, 1.f),
					DirectX::XMConvertToRadians(60.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 0.2f, 1000.f);
				tDesc.stCamValue = UCamera::CAMVALUE(20.f, DirectX::XMConvertToRadians(90.f));
				tDesc.eCamType = CAMERATYPE::MAIN;
				// Actor Add Main Camera

				VOIDDATAS vecDatas;
				vecDatas.push_back(&tDesc);

				SHPTR<CMainCamera> spMainCamera = std::static_pointer_cast<CMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
				spMainCamera->GetTransform()->SetPos({ 0.f, 10.f, -100.f });

				CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_WARRIORANIMCONTROLLER };
				CWarriorPlayer::PLAYERDESC PlayerDesc{ spMainCamera };
				SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_WARRIORPLAYER, { &CharDesc, &PlayerDesc }));

				spGameInstance->RegisterCurrentPlayer(spWarriorPlayer);
			}
		}
		break;
		case TAG_CHAR::TAG_OTHERPLAYER:
		{

		}
		break;
		}
	}
	__super::MakeActors();
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
			CHARDATA charData;
			VECTOR3 collidersize;
			::memcpy(&charData, &scConnectSuccess.chardata(), sizeof(CHARDATA));
			::memcpy(&collidersize, &scConnectSuccess.collidersize(), sizeof(VECTOR3));
			{
				CHARSTATUS status(charData.power(), charData.defensive(), charData.hp());
				_float3 vScale(collidersize.x(), collidersize.y(), collidersize.z());

				NETWORKRECEIVEINITDATA networkRecvInitData(scConnectSuccess.id(),
					status, scConnectSuccess.cellindex(), vScale, scConnectSuccess.type());
				// newwork Init Data 추가
				AddNetworkInitData(networkRecvInitData.iNetworkID, networkRecvInitData);
			}
			// 로그인에 성공했다는 패킷 보내기
			CS_LOGIN csLogin;
			PROTOFUNC::MakeCsLogin(OUT & csLogin, scConnectSuccess.id());
			SendProtoData(csLogin, TAG_CS::TAG_CS_LOGIN);
		}
		break; 
		case TAG_SC::TAG_SC_OTHERCLIENTLOGIN:
		{

		}
		break;
	}
#endif
}

void CNetworkClientController::Free()
{
}
