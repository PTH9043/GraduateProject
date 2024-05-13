#include "ClientDefines.h"
#include "CMainScene.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "ULight.h"
#include "UFire.h"
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


BEGIN(Client)

CMainScene::CMainScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice, (_uint)SCENE::SCENE_LOGO),
	m_spMainCamera{ nullptr },
	m_spMap{nullptr}
{
}

void CMainScene::Free()
{
}

HRESULT CMainScene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CProtoMaker::CreateMainSceneProtoData(spGameInstance, GetDevice(), std::static_pointer_cast<UCommand>(spGameInstance->GetGpuCommand()));
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
	}
	{
		m_spMap = CreateConstructorNative<CMap>(spGameInstance->GetDevice());
		m_spMap->LoadRooms();
		m_spMap->LoadStaticObjects();

		spGameInstance->TurnOnFog();

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 1.f}, {0.3f, 0.3f,0.3f, 1.f}, {0.15f, 0.15f, 0.15f, 1.f}, {0.f, -1.f, 0.f,}, {0.f, 100.f, 0.f}, 0.f, 0.f ,
	1.f, 20.f });


		//횃불의 조명 추가
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
	{
		CMummy::CHARACTERDESC CharDesc{PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_MUMMYANIMCONTROLLER};

		m_spMummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(
			PROTO_ACTOR_MUMMY, { &CharDesc }));
	}

	{
		CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_WARRIORANIMCONTROLLER};
		CWarriorPlayer::PLAYERDESC PlayerDesc{m_spMainCamera };
		m_spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
		PROTO_ACTOR_WARRIORPLAYER, {&CharDesc, &PlayerDesc }));
	}

	return S_OK;
}

void CMainScene::Tick(const _double& _dTimeDelta)
{
	_float3 PlayerPos = m_spWarriorPlayer->GetTransform()->GetPos();
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	
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
			
				
				if (distanceSq <= 180 *180)
				{
					torch_it->get()->SetActive(true);
					ActiveLIght(LIGHTTYPE::TYPE_POINT, count, LIGHTACTIVE::ISACTIVE);
					// dynamic_cast를 사용하여 자식 클래스로 캐스팅
					
					if (pTorch != nullptr)
					{
						// 자식 클래스로 캐스팅된 경우에만 GetParticle 함수 호출 가능
						pTorch->GetParticle()->SetActive(true);
						pTorch->GetFire()->SetActive(true);
					}
				}
				else {
					torch_it->get()->SetActive(false);
					ActiveLIght(LIGHTTYPE::TYPE_POINT, count, LIGHTACTIVE::NONACTIVE);
					if (pTorch != nullptr)
					{
						// 자식 클래스로 캐스팅된 경우에만 GetParticle 함수 호출 가능
						pTorch->GetParticle()->SetActive(false);
						pTorch->GetFire()->SetActive(false);
					}
				}
				torch_it++;
				count++;
			}
		}
	}



	


	SHPTR<ULight> DirLight;
	OutLight(LIGHTTYPE::TYPE_DIRECTIONAL, 0, DirLight);
	

	SHPTR<ULight> PointLight;
	OutLight(LIGHTTYPE::TYPE_POINT, 0, PointLight);
}

void CMainScene::LateTick(const _double& _dTimeDelta)
{

}

END
