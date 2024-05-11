#include "ClientDefines.h"
#include "CMainScene.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "ULight.h"
#include "UTransform.h"
#include "CMap.h"
#include "UStageManager.h"
#include "UStage.h"
#include "CWarriorPlayer.h"
#include "CProtoMaker.h"
#include "UGpuCommand.h"
#include "CRooms.h"
#include "UCollider.h"
#include "UMethod.h"
#include "CTorch.h"

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
		m_spMap->LoadNavigation();
		m_spMap->LoadStaticObjects();
	/*	for (auto& obj : (*m_spMap->GetStaticObjs().get()))
		{
			if (UMethod::ConvertWToS(obj.first) == "Torch_FBX.bin")
			{
				for(auto& torch : obj.second)
				{
					_float3 Pos = torch->GetTransform()->GetPos();
					AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTVERSION::TYPE_YONGBBA, {0.3f, 0.3f, 0.3f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.15f, 0.125f, 0.11f, 1.f}, {0.f, 0.f, 1.f,},
					Pos, 40.f, 0.f ,
					1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });
					m_spMap->AddLightCount();
				}
			}
		}*/
		//AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTVERSION::TYPE_YONGBBA, {0.3f, 0.3f, 0.3f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.15f, 0.125f, 0.11f, 1.f}, {0.f, 0.f, 1.f,},
		//			_float3{-167.f, -80.54f, 133.f}, 40.f, 0.f ,
		//			1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTVERSION::TYPE_YONGBBA, {0.3f, 0.3f, 0.3f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.15f, 0.125f, 0.11f, 1.f}, {0.f, 0.f, 1.f,},
					_float3{-190.f, -80.54f, 133.f}, 40.f, 0.f ,
					1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });

	}
	{
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL,LIGHTVERSION::TYPE_YONGBBA, {0.5f, 0.5f, 0.5f, 1.f}, {0.15f, 0.15f,0.15f, 1.f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.f, -1.f, 0.f,}, {0.f, 100.f, 0.f}, 0.f, 0.f ,
		1.f, 20.f });


		/*AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_SPOT,LIGHTVERSION::TYPE_YONGBBA, {0.3f, 0.3f, 0.3f, 0.f}, {0.2f, 0.f, 0.f, 1.f}, {1.f, 0.5f, 0.2f, 1.f}, {0.f, 0.f, 1.f,}
				, m_spMainCamera->GetTransform()->GetPos(), 100.f, 60.f ,
				100.f, 32.f, 8.0f,(float)cos(DirectX::XMConvertToRadians(30.f)),(float)cos(DirectX::XMConvertToRadians(15.f)),_float3(1.0f, 0.01f, 0.0001f) });

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTVERSION::TYPE_YONGBBA, {1.f, 0.f, 0.f, 1.f}, {0.f, 1.f, 0.f, 1.f}, {0.f, 0.f, 1.f, 1.f}, {0.f, 0.f, 1.f,},
				_float3(0,0,0), 30.f, 0.f ,
				1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });*/
	}

	{
		CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_WARRIORANIMCONTROLLER, 
			m_spMap->GetStageManager() };
		CWarriorPlayer::PLAYERDESC PlayerDesc{m_spMainCamera };
		m_spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
		PROTO_ACTOR_WARRIORPLAYER, {&CharDesc, &PlayerDesc }));
	}
	return S_OK;
}

void CMainScene::Tick(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	//SHPTR<ULight> SpotLight;
	//OutLight(LIGHTTYPE::TYPE_SPOT, 0, SpotLight);
	//SpotLight->SetLightPos(m_spMainCamera->GetTransform()->GetPos());
	//SpotLight->SetDirection(m_spMainCamera->GetTransform()->GetLook());
	//if (pGameInstance->GetDIKeyPressing(DIK_1))
	//	SpotLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	//if (pGameInstance->GetDIKeyPressing(DIK_2))
	//	SpotLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);
	//if (pGameInstance->GetDIKeyPressing(DIK_3))
	//	SpotLight->SetLightVersion(LIGHTVERSION::TYPE_END);

	//SHPTR<ULight> PointLight;
	//OutLight(LIGHTTYPE::TYPE_POINT, 0, PointLight);
	//if (pGameInstance->GetDIKeyPressing(DIK_6))
	//	PointLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	//if (pGameInstance->GetDIKeyPressing(DIK_7))
	//	PointLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);
	//if (pGameInstance->GetDIKeyPressing(DIK_8))
	//	PointLight->SetLightVersion(LIGHTVERSION::TYPE_END);
	//_float3 pos = m_spMainCamera->GetTransform()->GetPos();
	//PointLight->SetLightPos(_float3(pos.x, pos.y, pos.z));

	SHPTR<ULight> DirLight;
	OutLight(LIGHTTYPE::TYPE_DIRECTIONAL, 0, DirLight);
	if (pGameInstance->GetDIKeyPressing(DIK_4))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	if (pGameInstance->GetDIKeyPressing(DIK_5))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);

	//for (int i = 0; i < m_spMap->GetLightCount(); i++)
	//{
	//	SHPTR<ULight> PointLight;
	//	OutLight(LIGHTTYPE::TYPE_POINT, i, PointLight);
	//}

}

void CMainScene::LateTick(const _double& _dTimeDelta)
{
	m_spMap->GetStageManager()->GetStage()->AddRender(0);
	if (nullptr != m_spMap->GetStageManager()->GetStage())
		m_spMap->GetStageManager()->GetStage()->UpdateRegion();
}

END
