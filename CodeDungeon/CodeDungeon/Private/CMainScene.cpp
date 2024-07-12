#include "ClientDefines.h"
#include "CMainScene.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "ULight.h"
#include "UFire.h"
#include "UGuard.h"
#include "UTransform.h"
#include "ULight.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "CMap.h"
#include "UStageManager.h"
#include "UStage.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "CWarriorPlayer.h"
#include "CProtoMaker.h"
#include "UGpuCommand.h"
#include "CRooms.h"
#include "UCollider.h"
#include "UMethod.h"
#include "CTorch.h"
#include "CMummy.h"
#include "CSarcophagus.h"
#include "UAnimModel.h"
#include "CIronBars.h"
#include "CModelObjects.h"


BEGIN(Client)

CMainScene::CMainScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice, SCENE::SCENE_STAGE1),
	m_spMainCamera{ nullptr },
	m_spMap{nullptr}
{
}

void CMainScene::TurnMobsOnRange()
{
	_float3 PlayerPos = m_spMainCamera->GetTransform()->GetPos();
	for (auto& mob : (*m_spMap->GetMobs().get()))
	{
		auto mob_it = mob.second.begin();
		while (mob_it != mob.second.end())
		{
			_float3 mobPos = mob_it->get()->GetTransform()->GetPos();
			_float3 distance = mobPos - PlayerPos;
			float distanceSq = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
			
			if (distanceSq <= 200 * 200)
			{
				mob_it->get()->SetActive(true);
			}
			else {
				mob_it->get()->SetActive(false);
			}
			mob_it++;
		}
	}


}
void CMainScene::TurnLightsOnRange()
{
	_float3 PlayerPos = m_spMainCamera->GetTransform()->GetPos();
	for (auto& obj : (*m_spMap->GetStaticObjs().get()))
	{
		int count = 0;
		if (UMethod::ConvertWToS(obj.first) == "Torch_FBX.bin")
		{
			auto torch_it = obj.second.begin();
			while (torch_it != obj.second.end())
			{
				_float3 torchPos = torch_it->get()->GetTransform()->GetPos();
				_float3 distance = torchPos - PlayerPos;
				float distanceSq = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
				CTorch* pTorch = dynamic_cast<CTorch*>(torch_it->get());

				if (distanceSq <= 120 * 120) {
					pTorch->GetParticle()->SetActive(true);
				}
				else {
					pTorch->GetParticle()->SetActive(false);
				}
				if (distanceSq <= 200 * 200)
				{
					torch_it->get()->SetActive(true);
					ActiveLIght(LIGHTTYPE::TYPE_POINT, count, LIGHTACTIVE::ISACTIVE);
					if (pTorch != nullptr)
					{
						pTorch->GetFire()->SetActive(true);
					}
				}
				else {
					torch_it->get()->SetActive(false);
					ActiveLIght(LIGHTTYPE::TYPE_POINT, count, LIGHTACTIVE::NONACTIVE);
					if (pTorch != nullptr)
					{			
						pTorch->GetFire()->SetActive(false);
					}
				}
				torch_it++;
				count++;
			}
		}
	}
}

void CMainScene::TurnRoomsOnRange()
{
	_float3 PlayerPos = m_spMainCamera->GetTransform()->GetPos();
	for (auto& Rooms : (*m_spMap->GetRooms().get()))
	{
		_float3 roomPos = Rooms.second->GetRoomCenterPos();
		_float3 distance = roomPos - PlayerPos;
		float distanceSq = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
		if (distanceSq <= 400 * 400)
		{
			Rooms.second->SetActive(true);
		}
		else
		{
			Rooms.second->SetActive(false);
		}
	}
}

void CMainScene::Free()
{
}

HRESULT CMainScene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CProtoMaker::CreateMainSceneProtoData(spGameInstance, GetDevice(), std::static_pointer_cast<UCommand>(spGameInstance->GetGpuCommand()));
#ifdef _ENABLE_PROTOBUFF
	UCamera::CAMDESC tDesc;
	tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::PERSPECTIVE, _float3(0.f, 0.f, 0.f),
		_float3(0.f, 0.f, 1.f),
		DirectX::XMConvertToRadians(60.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 0.2f, 1000.f);
	tDesc.stCamValue = UCamera::CAMVALUE(20.f, DirectX::XMConvertToRadians(90.f));
	tDesc.eCamType = CAMERATYPE::MAIN;
	// Actor Add Main Camera

	VOIDDATAS vecDatas;
	vecDatas.push_back(&tDesc);

	m_spMainCamera = std::static_pointer_cast<CMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
	m_spMainCamera->GetTransform()->SetPos({ 0.f, 10.f, -100.f });

	spGameInstance->MakeActors({ m_spMainCamera }, nullptr);
#endif
	{
		m_spMap = CreateConstructorNative<CMap>(spGameInstance->GetDevice());
		m_spMap->LoadRooms();
		m_spMap->LoadStaticObjects();
		spGameInstance->TurnOnFog();

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 1.f}, {0.3f, 0.3f,0.3f, 1.f}, {0.15f, 0.15f, 0.15f, 1.f}, {0.f, -1.f, 0.f,}, {0.f, 100.f, 0.f}, 0.f, 0.f ,
	1.f, 20.f });

		for (auto& obj : (*m_spMap->GetStaticObjs().get()))
		{
			int count = 0;
			if (UMethod::ConvertWToS(obj.first) == "Torch_FBX.bin")
			{
				auto torch_it = obj.second.begin();
				while (torch_it != obj.second.end())
				{
					AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.8f, 0.4f, 0.16f, 1.f}, {0.f, 0.f, 1.f,},
						_float3(torch_it->get()->GetTransform()->GetPos().x,torch_it->get()->GetTransform()->GetPos().y + 4,torch_it->get()->GetTransform()->GetPos().z), 40.f, 0.f ,
					1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });
					m_spMap->AddLightCount();
					torch_it++;
				}
			}
		}
	}
#ifndef _ENABLE_PROTOBUFF
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

			m_spMainCamera = std::static_pointer_cast<CMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
			m_spMainCamera->GetTransform()->SetPos({ 0.f, 10.f, -100.f });

			CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_USERWARRIORANIMCONTROLLER };
			CWarriorPlayer::PLAYERDESC PlayerDesc{ m_spMainCamera };
			m_spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_WARRIORPLAYER, { &CharDesc, &PlayerDesc }));
			spGameInstance->RegisterCurrentPlayer(m_spWarriorPlayer);
		}
	}

	m_spMap->LoadMobs(m_spWarriorPlayer);

	//{
	//	CMummy::CHARACTERDESC CharDesc{PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_MUMMYANIMCONTROLLER};
	//	m_spMummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(
	//		PROTO_ACTOR_MUMMY, { &CharDesc }));
	//	m_spMummy->SetMummyType(CMummy::MUMMYTYPE::TYPE_LYING);
	//	m_spMummy->GetAnimModel()->SetAnimation(L"staticLaying");
	//	m_spMummy->SetTargetPlayer(m_spWarriorPlayer);
	//	m_spMummy->SetMobPlacement(588);
	//	spGameInstance->AddCollisionPawnList(m_spMummy);
	//}

	//{
	//	CSarcophagus::CHARACTERDESC CharDesc{ PROTO_RES_SARCOPHAGUSLYINGANIMMODEL, PROTO_COMP_SARCOPHAGUSANIMCONTROLLER };
	//	m_spSarcophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
	//		PROTO_ACTOR_SARCOPHAGUSLYING, { &CharDesc }));
	//	m_spSarcophagus->SetSarcophagusType(CSarcophagus::SARCOTYPE::TYPE_LYING);
	//	m_spSarcophagus->GetTransform()->SetNewWorldMtx(m_spMummy->GetTransform()->GetWorldMatrix());
	//	m_spSarcophagus->GetAnimModel()->SetAnimation(0);
	//	m_spSarcophagus->SetTargetPlayer(m_spWarriorPlayer);
	//	//�̶� ��ġ����
	//	m_spMummy->GetTransform()->TranslateDir((m_spMummy->GetTransform()->GetLook()), 1, 10);
	//}
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		m_stGuard = std::static_pointer_cast<UGuard>(spGameInstance->CloneActorAdd(PROTO_ACTOR_GUARD));
		m_stGuard->SetActive(true);
		m_stGuard->SetColorTexture(L"asdf");
	}
#endif
	return S_OK;
}

void CMainScene::Tick(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	TurnLightsOnRange();
	TurnRoomsOnRange();
	TurnMobsOnRange();


	SHPTR<ULight> DirLight;
	OutLight(LIGHTTYPE::TYPE_DIRECTIONAL, 0, DirLight);
	

	SHPTR<ULight> PointLight;
	OutLight(LIGHTTYPE::TYPE_POINT, 0, PointLight);

	if(pGameInstance->GetDIKeyDown(DIK_ESCAPE))
		::PostQuitMessage(0);
}

void CMainScene::LateTick(const _double& _dTimeDelta)
{

}

void CMainScene::CollisionTick(const _double& _dTimeDelta)
{
	
}
END
