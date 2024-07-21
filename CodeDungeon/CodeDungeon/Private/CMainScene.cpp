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
#include "CImageUI.h"
#include "CButtonUI.h"
#include "CLoadingUI.h"
#include "CHpBarUI.h"

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
void CMainScene::TurnGuardsOnRange()
{
	_float3 PlayerPos = m_spMainCamera->GetTransform()->GetPos();
	for (auto& mob : m_spMap->GetGuards())
	{
		_float3 mobPos = mob.second->GetTransform()->GetPos();
		_float3 distance = mobPos - PlayerPos;
		float distanceSq = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;

		if (distanceSq <= 200 * 200)
		{
			mob.second->SetActive(true);
		}
		else {
			mob.second->SetActive(false);
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

	
	
		
		/*_float3 lightPos0 = _float3(-364.225, -20, 253.010);
		_float3 lightPos1 = _float3(-535.39, -20, 154.5);
		_float3 lightPos2 = _float3(-494.5, -45, 289.265);
		
		_float3 distance = lightPos1 - PlayerPos;
		float distanceSq = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
		if (distanceSq <= 50 * 50)
		{
			for (int i = 0; i < 3; i++) {

				ActiveLIght(LIGHTTYPE::TYPE_SPOT, i, LIGHTACTIVE::ISACTIVE);
			}
		}
		else
		{
			for (int i = 0; i < 3; i++) {

				ActiveLIght(LIGHTTYPE::TYPE_SPOT, i, LIGHTACTIVE::NONACTIVE);
			}
			
		}*/
	
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


void CMainScene::CreateStartSceneUI()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	{
		CImageUI::UIDESC tDesc;
		{
			tDesc.fZBufferOrder = 0.99f;
			tDesc.strImgName = L"Background";
			tDesc._shaderName = PROTO_RES_BACKGROUNDUISHADER;
			tDesc.DrawOrder = L"Priority";
			tDesc.v2Size.x = static_cast<_float>(1280);
			tDesc.v2Size.y = static_cast<_float>(1080);
			tDesc.v2Pos = _float2{ 640,540 };
			m_spBackgroundUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
			m_spBackgroundUI->SetActive(true);
		}
		CImageUI::UIDESC tDesc1;
		{
			// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
			tDesc1.fZBufferOrder = 0.97f;
			tDesc1.strImgName = L"LoadingBar_Background";
			tDesc1._shaderName = PROTO_RES_DEFAULTUISHADER;
			tDesc1.DrawOrder = L"Middle";
			tDesc1.v2Size.x = static_cast<_float>(1020);
			tDesc1.v2Size.y = static_cast<_float>(27);
			tDesc1.v2Pos = _float2{ 640,840 };
			m_spLoadingBackgroundUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc1 }));
			m_spLoadingBackgroundUI->SetActive(false);
		}
		CButtonUI::UIDESC tDesc2;
		{
			tDesc2.fZBufferOrder = 0.97f;
			tDesc2.strImgName = L"LoadingBar_Fill";
			tDesc2._shaderName = PROTO_RES_LOADINGUISHADER;
			tDesc2.DrawOrder = L"Last";
			tDesc2.v2Size.x = static_cast<_float>(1000);
			tDesc2.v2Size.y = static_cast<_float>(25);
			tDesc2.v2Pos = _float2{ 640,840 };
			m_spLoadingFillingUI = std::static_pointer_cast<CLoadingUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_LOADINGUI, { &tDesc2 }));
			m_spLoadingFillingUI->SetActive(false);
		}
		CImageUI::UIDESC tDesc3;
		{
			// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
			tDesc3.fZBufferOrder = 0.88f;
			tDesc3.strImgName = L"MainTitle";
			tDesc3._shaderName = PROTO_RES_DEFAULTUISHADER;
			tDesc3.DrawOrder = L"Last";
			tDesc3.v2Size.x = static_cast<_float>(640);
			tDesc3.v2Size.y = static_cast<_float>(240);
			tDesc3.v2Pos = _float2{ 640,220 };
			m_spMainTitleUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc3 }));
			m_spMainTitleUI->SetActive(true);
		}
		CButtonUI::UIDESC tDesc4;
		{
			tDesc4.fZBufferOrder = 0.47f;
			tDesc4.strImgName = L"Enter";
			tDesc4._shaderName = PROTO_RES_BUTTONUISHADER;
			tDesc4.DrawOrder = L"Last";
			tDesc4.v2Size.x = static_cast<_float>(200);
			tDesc4.v2Size.y = static_cast<_float>(100);
			tDesc4.v2Pos = _float2{ 640, 650 } ;
			m_spEnterButtonUI = std::static_pointer_cast<CButtonUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_BUTTONUI, { &tDesc4 }));
			m_spEnterButtonUI->SetActive(true);
		}
		{
			tDesc4.fZBufferOrder = 0.47f;
			tDesc4.strImgName = L"Exit";
			tDesc4._shaderName = PROTO_RES_BUTTONUISHADER;
			tDesc4.DrawOrder = L"Last";
			tDesc4.v2Size.x = static_cast<_float>(180);
			tDesc4.v2Size.y = static_cast<_float>(80);
			tDesc4.v2Pos = _float2{ 640, 770 };
			m_spExitButtonUI = std::static_pointer_cast<CButtonUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_BUTTONUI, { &tDesc4 }));
			m_spExitButtonUI->SetActive(true);
		}
		CImageUI::UIDESC tDesc5;
		{
			// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
			tDesc5.fZBufferOrder = 0.8f;
			tDesc5.strImgName = L"T_TitleLogo_Shadow_SmokeWave_UI";
			tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
			tDesc5.DrawOrder = L"Middle";
			tDesc5.v2Size.x = static_cast<_float>(640);
			tDesc5.v2Size.y = static_cast<_float>(250);
			tDesc5.v2Pos = _float2{ 640,212.5 };
			m_spMainTitleEffectUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
			m_spMainTitleEffectUI->SetActive(true);
		}
		
		{
			// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
			tDesc5.fZBufferOrder = 0.48f;
			tDesc5.strImgName = L"T_TitleLogo_Shadow_Line_UI";
			tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
			tDesc5.DrawOrder = L"Last";
			tDesc5.v2Size.x = static_cast<_float>(1080);
			tDesc5.v2Size.y = static_cast<_float>(25);
			tDesc5.v2Pos = _float2{ 640,870 };
			m_spLineEffectUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
			m_spLineEffectUI->SetActive(true);
		}
		{
			// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
			tDesc5.fZBufferOrder = 0.43f;
			tDesc5.strImgName = L"Loading";
			tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
			tDesc5.DrawOrder = L"Last";
			tDesc5.v2Size.x = static_cast<_float>(150);
			tDesc5.v2Size.y = static_cast<_float>(50);
			tDesc5.v2Pos = _float2{ 1050,800 };
			m_spLoadingTextUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
			m_spLoadingTextUI->SetActive(false);
		}
		{
			// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
			tDesc5.fZBufferOrder = 0.43f;
			tDesc5.strImgName = L"LoadingDot";
			tDesc5._shaderName = PROTO_RES_LOADINGDOTUISHADER;
			tDesc5.DrawOrder = L"Last";
			tDesc5.v2Size.x = static_cast<_float>(60);
			tDesc5.v2Size.y = static_cast<_float>(25);
			tDesc5.v2Pos = _float2{ 1145,805 };
			m_spLoadingDotsUI = std::static_pointer_cast<CLoadingUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_LOADINGUI, { &tDesc5 }));
			m_spLoadingDotsUI->SetActive(false);
		}
		{
			// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
			tDesc5.fZBufferOrder = 0.43f;
			tDesc5.strImgName = L"PleaseWait";
			tDesc5._shaderName = PROTO_RES_PLEASEWAITUISHADER;
			tDesc5.DrawOrder = L"Last";
			tDesc5.v2Size.x = static_cast<_float>(200);
			tDesc5.v2Size.y = static_cast<_float>(75);
			tDesc5.v2Pos = _float2{ 640, 650 };
			m_spPleaseWaitTextUI = std::static_pointer_cast<CLoadingUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_LOADINGUI, { &tDesc5 }));
			m_spPleaseWaitTextUI->SetActive(false);
		}
	}
}

void CMainScene::CreateGameSceneUI()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CImageUI::UIDESC tDesc5;
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc5.fZBufferOrder = 0.43f;
		tDesc5.strImgName = L"Player";
		tDesc5._shaderName = PROTO_RES_HPBARUISHADER;
		tDesc5.DrawOrder = L"Last";
		tDesc5.v2Size.x = static_cast<_float>(350);
		tDesc5.v2Size.y = static_cast<_float>(32.5);
		tDesc5.v2Pos = _float2{ 250, 900 };
		m_spHpBarUI = std::static_pointer_cast<CHpBarUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_HPBARUI, { &tDesc5 }));
		m_spHpBarUI->SetActive(false);
		m_spHpBarUI->SetMaxHp(500.f);
		m_spHpBarUI->SetCurHp(258.f);
	}
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc5.fZBufferOrder = 0.43f;
		tDesc5.strImgName = L"UnitFrame_Dragon";
		tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
		tDesc5.DrawOrder = L"Priority";
		tDesc5.v2Size.x = static_cast<_float>(142.5);
		tDesc5.v2Size.y = static_cast<_float>(142.5);
		tDesc5.v2Pos = _float2{ 110,832.5 };
		m_spBackDragonPlayerFrameUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
		m_spBackDragonPlayerFrameUI->SetActive(false);
	}
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc5.fZBufferOrder = 0.43f;
		tDesc5.strImgName = L"UnitFrame_Avatar_Background";
		tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
		tDesc5.DrawOrder = L"Middle";
		tDesc5.v2Size.x = static_cast<_float>(80);
		tDesc5.v2Size.y = static_cast<_float>(80);
		tDesc5.v2Pos = _float2{ 115,840 };
		m_spBackPlayerFrameUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
		m_spBackPlayerFrameUI->SetActive(false);
	}
		
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc5.fZBufferOrder = 0.43f;
		tDesc5.strImgName = L"UnitFrame_Avatar_Example";
		tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
		tDesc5.DrawOrder = L"Last";
		tDesc5.v2Size.x = static_cast<_float>(75);
		tDesc5.v2Size.y = static_cast<_float>(75);
		tDesc5.v2Pos = _float2{ 115,840 };
		m_spFrontPlayerFrameUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
		m_spFrontPlayerFrameUI->SetActive(false);
	}
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc5.fZBufferOrder = 0.43f;
		tDesc5.strImgName = L"PlayerLogo";
		tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
		tDesc5.DrawOrder = L"Last";
		tDesc5.v2Size.x = static_cast<_float>(100);
		tDesc5.v2Size.y = static_cast<_float>(45);
		tDesc5.v2Pos = _float2{ 220,865 };
		m_spPlayerNameUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
		m_spPlayerNameUI->SetActive(false);
	}
	
	//=====================Minotaur UI=========================
	CImageUI::UIDESC tDesc1;
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc1.fZBufferOrder = 0.43f;
		tDesc1.strImgName = L"Boss";
		tDesc1._shaderName = PROTO_RES_HPBARUISHADER;
		tDesc1.DrawOrder = L"Middle";
		tDesc1.v2Size.x = static_cast<_float>(1000);
		tDesc1.v2Size.y = static_cast<_float>(35);
		tDesc1.v2Pos = _float2{ 640, 50 };
		m_spMinotaurHpBarUI = std::static_pointer_cast<CHpBarUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_HPBARUI, { &tDesc1 }));
		m_spMinotaurHpBarUI->SetActive(false);
		m_spMinotaurHpBarUI->SetMaxHp(500.f);
		m_spMinotaurHpBarUI->SetCurHp(258.f);
	}
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc5.fZBufferOrder = 0.43f;
		tDesc5.strImgName = L"MinotaurFrame";
		tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
		tDesc5.DrawOrder = L"Middle";
		tDesc5.v2Size.x = static_cast<_float>(80);
		tDesc5.v2Size.y = static_cast<_float>(80);
		tDesc5.v2Pos = _float2{ 100,50 };
		m_spMinotaurFrameUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc5 }));
		m_spMinotaurFrameUI->SetActive(false);
	}
}

HRESULT CMainScene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CProtoMaker::CreateMainSceneProtoData(spGameInstance, GetDevice(), std::static_pointer_cast<UCommand>(spGameInstance->GetGpuCommand()));
	

	CreateStartSceneUI();
	CreateGameSceneUI();
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

	spGameInstance->MakeActors({ m_spMainCamera });
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

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_SPOT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 0.f}, {0.5f, 0.25f, 0.11f, 1.f}, {0.f, 1.5f, 1.2f, 1.f}, {0.f, -1.f, 0.f,}
			, _float3(-364.225,-20,253.010), 100.f, 60.f ,
			100.f, 32.f, 8.0f,(float)cos(DirectX::XMConvertToRadians(30.f)),(float)cos(DirectX::XMConvertToRadians(15.f)),_float3(1.0f, 0.01f, 0.0001f) });

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_SPOT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 0.f}, {0.5f, 0.25f, 0.11f, 1.f}, {0.f, 1.5f, 1.2f, 1.f}, {0.f, -1.f, 0.f,}
			, _float3(-535.39,-20,154.5), 100.f, 60.f ,
			100.f, 32.f, 8.0f,(float)cos(DirectX::XMConvertToRadians(30.f)),(float)cos(DirectX::XMConvertToRadians(15.f)),_float3(1.0f, 0.01f, 0.0001f) });

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_SPOT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 0.f}, {0.5f, 0.25f, 0.11f, 1.f}, {0.f, 1.5f, 1.2f, 1.f}, {0.f, -1.f, 0.f,}
			, _float3(-494.5,-45,289.265), 100.f, 60.f ,
			100.f, 32.f, 8.0f,(float)cos(DirectX::XMConvertToRadians(45.f)),(float)cos(DirectX::XMConvertToRadians(30.f)),_float3(1.0f, 0.01f, 0.0001f) });


	}
	{
		UFire::FIREDESC tFireDesc;
		tFireDesc.wstrFireShader = PROTO_RES_2DFIRESHADER;
		m_stFireOne = std::static_pointer_cast<UFire>(spGameInstance->CloneActorAdd(PROTO_ACTOR_FIRE, { &tFireDesc }));
		m_stFireTwo = std::static_pointer_cast<UFire>(spGameInstance->CloneActorAdd(PROTO_ACTOR_FIRE, { &tFireDesc }));
		
		 float ScaleX = 1.99f;
		 float ScaleY = 3.64f;
		_float3 ScaleFloat3 = _float3(ScaleX, ScaleY, 1);

		m_stFireOne->GetTransform()->SetScale(ScaleFloat3);
		m_stFireOne->GetTransform()->SetPos(_float3(-438.7,-50.8,156.4));
		m_stFireOne->SetColorTexture(L"BlueFlame");
		m_stFireOne->SetNoiseTexture(L"WFX_T_NukeFlames");
		m_stFireTwo->GetTransform()->SetScale(ScaleFloat3);
		m_stFireTwo->GetTransform()->SetPos(_float3(-410.6, -50.8, 172.8));
		m_stFireTwo->SetColorTexture(L"BlueFlame");
		m_stFireTwo->SetNoiseTexture(L"WFX_T_NukeFlames");
		m_stFireOne->SetActive(true);
			m_stFireTwo->SetActive(true);
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

			CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_USERWARRIORANIMCONTROLLER };
			CWarriorPlayer::PLAYERDESC PlayerDesc{ m_spMainCamera };
			m_spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_WARRIORPLAYER, { &CharDesc, &PlayerDesc }));
			spGameInstance->RegisterCurrentPlayer(m_spWarriorPlayer);
			m_spMainCamera->GetTransform()->SetPos(m_spWarriorPlayer->GetTransform()->GetPos());
		
		}
	}

	m_spMap->LoadMobs(m_spWarriorPlayer);
	m_spMap->LoadGuards();
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
	//{
	//	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	//	m_stGuard = std::static_pointer_cast<UGuard>(spGameInstance->CloneActorAdd(PROTO_ACTOR_GUARD));
	//	m_stGuard->SetActive(true);
	//	m_stGuard->SetColorTexture(L"asdf");
	//}
#endif
	return S_OK;
}

void CMainScene::DrawStartSceneUI(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	if (m_spEnterButtonUI->IsMouseOnRect()&& m_spEnterButtonUI->IsActive()) {

		m_spEnterButtonUI->SetIfPicked(true);

		if (true == spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_L)&&!m_bStartSceneForUI) {
			m_bStartSceneForUI = true;		
		}
	}
	else {
		m_spEnterButtonUI->SetIfPicked(false);
	}
	if (m_spExitButtonUI->IsMouseOnRect()&& m_spExitButtonUI->IsActive()) {

		m_spExitButtonUI->SetIfPicked(true);

		if (true == spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_L)) {
			::PostQuitMessage(0);
		}
	}
	else {
		m_spExitButtonUI->SetIfPicked(false);
	}

	if (m_bStartSceneForUI&& !m_bStartGameForUI) {
		m_fStartSceneLoadingTimer += _dTimeDelta;
		m_spEnterButtonUI->SetActive(false);
		m_spLoadingBackgroundUI->SetActive(true);
		m_spLoadingFillingUI->SetActive(true);
		m_spLoadingFillingUI->SetIfPicked(true);
		m_spLoadingTextUI->SetActive(true);
		
		m_spLoadingDotsUI->SetActive(true);
		m_spLoadingDotsUI->SetIfPicked(true);
		m_spPleaseWaitTextUI->SetActive(true);
		m_spPleaseWaitTextUI->SetIfPicked(true);


	}

	if (m_fStartSceneLoadingTimer > 10.f) {

		spGameInstance->SetGameStartEffect();
		m_spBackgroundUI->SetActive(false);
		m_spMainTitleUI->SetActive(false);
		m_spLoadingTextUI->SetActive(false);
		m_spLineEffectUI->SetActive(false);
		m_spMainTitleEffectUI->SetActive(false);
		m_spLoadingDotsUI->SetActive(false);
		m_spPleaseWaitTextUI->SetActive(false);
		m_spLoadingFillingUI->SetActive(false);
		m_spLoadingBackgroundUI->SetActive(false);
		m_spEnterButtonUI->SetActive(false);
		m_spExitButtonUI->SetActive(false);
		m_fStartSceneLoadingTimer = 0.f;
		m_bStartSceneForUI = false;
		m_bStartGameForUI = true;
	}

	if (!m_bStartSceneForUI && m_bStartGameForUI) {
		m_spHpBarUI->SetActive(true);
		m_spBackPlayerFrameUI->SetActive(true);
		m_spBackDragonPlayerFrameUI->SetActive(true);
		m_spFrontPlayerFrameUI->SetActive(true);
		m_spPlayerNameUI->SetActive(true);

		//==========Minotaur Hp===============
		m_spMinotaurHpBarUI->SetActive(true);
		m_spMinotaurFrameUI->SetActive(true);
	}
}


void CMainScene::Tick(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	DrawStartSceneUI(_dTimeDelta);
	TurnLightsOnRange();
	TurnRoomsOnRange();
	TurnGuardsOnRange();

	SHPTR<ULight> DirLight;
	OutLight(LIGHTTYPE::TYPE_DIRECTIONAL, 0, DirLight);
	
	if (pGameInstance->GetDIKeyDown(DIK_F6)) {
		pGameInstance->PauseGame();
	}
	if (pGameInstance->GetDIKeyDown(DIK_F7)) {
		pGameInstance->ResumeGame();
	}
	SHPTR<ULight> PointLight;
	OutLight(LIGHTTYPE::TYPE_POINT, 0, PointLight);

	if(pGameInstance->GetDIKeyDown(DIK_ESCAPE))
		::PostQuitMessage(0);
}

void CMainScene::LateTick(const _double& _dTimeDelta)
{
	TurnMobsOnRange();
}

void CMainScene::CollisionTick(const _double& _dTimeDelta)
{
	
}

END
