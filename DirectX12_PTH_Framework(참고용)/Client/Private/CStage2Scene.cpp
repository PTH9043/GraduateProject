#include "ClientDefine.h"
#include "CStage2Scene.h"
#include "UGameInstance.h"
#include "CPlayer.h"
#include "UMirror.h"
#include "UTransform.h"
#include "UTerrain.h"
#include "CMainCamera.h"
#include"CProtoMaker.h"
#include "UGpuCommand.h"
#include "USkyBox.h"
#include "CStage1Scene.h"

CStage2Scene::CStage2Scene(CSHPTRREF<UDevice> _spDevice)
	: UScene(_spDevice, SCENE::SCENE_STAGE2)
{
}

void CStage2Scene::Free()
{
}

HRESULT CStage2Scene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<UCommand> spCommand = static_pointer_cast<UCommand>(spGameInstance->GetGpuCommand());
	CProtoMaker::LoadStageScene2(spGameInstance, GetDevice(), spCommand);
	{
		{
			USkyBox::SKYBOXDESC tDesc{ L"SkyBox", L"SkyBox_Cloud", true };
			VOIDDATAS vecDatas;
			{
				vecDatas.push_back(&tDesc);
			}
			m_spSkyBox = static_pointer_cast<USkyBox>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SKYBOX, vecDatas));
		}
		// Camera
		{
			UCamera::CAMDESC tDesc;
			tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::PERSPECTIVE, _float3(0.f, 0.f, 0.f),
				_float3(0.f, 0.f, 1.f),
				DirectX::XMConvertToRadians(60.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 0.2f, 10000.f);
			tDesc.stCamValue = UCamera::CAMVALUE(5.f, DirectX::XMConvertToRadians(90.f));
			tDesc.eCamType = CAMERATYPE::MAIN;
			// Actor Add Main Camera
			VOIDDATAS vecDatas;
			{
				vecDatas.push_back(&tDesc);
			//	vecDatas.push_back(&m_spTerrainObject);
			}
			m_spMainCamera = static_pointer_cast<CMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
		}
		//{
		//	{
		//		// Default로 CloneOnce니까 주의
		//		UTerrain::TERRAINDESC tDesc;
		//		tDesc.wstrBufferProtoName = PROTO_RES_VIBUFFERTERRAIN;
		//		tDesc.wstrDiffuseTexName = L"Detail_Texture_7";
		//		tDesc.wstrHeightTexName = L"height";
		//		tDesc.wstrTextureProtoName = PROTO_RES_TERRAINGROUP;
		//		// Terrain 생성
		//		VOIDDATAS vecDatas;
		//		{
		//			vecDatas.push_back(&tDesc);
		//		}

		//		m_spTerrainObject = std::static_pointer_cast<UTerrain>(spGameInstance->CloneActorAdd(PROTO_ACTOR_TERRAIN, vecDatas));
		//		m_spTerrainObject->GetTransform()->SetScale(_float3{ 5.f, 5.f, 5.f });
		//		m_spTerrainObject->GetTransform()->SetPos(_float3{ -50.f, 0.f, -50.f });
		//	}
		//}

		{
			// -x
			SHPTR<UMirror> spMirror;
			spMirror = std::static_pointer_cast<UMirror>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MIRROR));
			spMirror->GetTransform()->SetPos(_float3{ 30.f, 0.f, 0.f });
			spMirror->GetTransform()->RotateTurn(_float3(0.f, 90.f, 0.f));
			spMirror->GetTransform()->SetScale(_float3{ 50.f, 50.f, 50.f });
			m_MirrorContainer.emplace_back(spMirror);

			spMirror = std::static_pointer_cast<UMirror>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MIRROR));
			spMirror->GetTransform()->SetPos(_float3{ -30.f, 0.f, 0.f });
			spMirror->GetTransform()->RotateTurn(_float3(0.f, -90.f, 0.f));
			spMirror->GetTransform()->SetScale(_float3{ 50.f, 50.f, 50.f });
			m_MirrorContainer.emplace_back(spMirror);


			spMirror = std::static_pointer_cast<UMirror>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MIRROR));
			spMirror->GetTransform()->SetPos(_float3{ 0.f, 0.f, 30.f });
			spMirror->GetTransform()->RotateTurn(_float3(0.f, 0.f, 0.f));
			spMirror->GetTransform()->SetScale(_float3{ 50.f, 50.f, 50.f });
			m_MirrorContainer.emplace_back(spMirror);


			spMirror = std::static_pointer_cast<UMirror>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MIRROR));
			spMirror->GetTransform()->SetPos(_float3{ 0.f, 0.f, -30.f });
			spMirror->GetTransform()->RotateFix(_float3(0.f, 180.f, 0.f));
			spMirror->GetTransform()->SetScale(_float3{ 50.f, 50.f, 50.f });
			m_MirrorContainer.emplace_back(spMirror);


			spMirror = std::static_pointer_cast<UMirror>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MIRROR));
			spMirror->GetTransform()->SetPos(_float3{ 0.f, 30.f, 0.f });
			spMirror->GetTransform()->RotateTurn(_float3(-90.f, 0.f, 0.f));
			spMirror->GetTransform()->SetScale(_float3{ 50.f, 50.f, 50.f });


			spMirror = std::static_pointer_cast<UMirror>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MIRROR));
			spMirror->GetTransform()->SetPos(_float3{ 0.f, -30.f, 0.f });
			spMirror->GetTransform()->RotateTurn(_float3(90.f, 0.f, 0.f));
			spMirror->GetTransform()->SetScale(_float3{ 50.f, 50.f, 50.f });
			m_MirrorContainer.emplace_back(spMirror);
		}

		{
			VOIDDATAS vecDatas;
			{
				vecDatas.push_back(&m_spTerrainObject);
				vecDatas.push_back(&m_spMainCamera);
			}
			m_spPlayer = static_pointer_cast<CPlayer>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PLAYER, vecDatas));
		}
	}
	{
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL, {1.f, 1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f, 0.05f}, {1.f, 1.f, 1.f, 1.f}, {0.f, -1.f, -1.f,},
			{0.f, 100.f, 0.f}, 0.f, 0.f, 1.f, 200.f },
			LIGHTCONTROL{ RIMLIGHT{} });
	}
	return S_OK;
}

void CStage2Scene::Tick(const _double& _dTimeDelta)
{
}

void CStage2Scene::LateTick(const _double& _dTimeDelta)
{
	SHPTR<UTransform> spTransform = m_spPlayer->GetTransform();
	if (spTransform->GetPos().x >= 30 || spTransform->GetPos().x <= -30)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		spGameInstance->RegisterScene(CreateConstructorNative<CStage1Scene>(GetDevice()));
	}

	if (spTransform->GetPos().z >= 30 || spTransform->GetPos().z <= -30)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		spGameInstance->RegisterScene(CreateConstructorNative<CStage1Scene>(GetDevice()));
	}
}

void CStage2Scene::CollisionTick(const _double& _dTimedelta)
{
}
