#include "ClientDefine.h"
#include "CStage1Scene.h"
#include "USkyBox.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UTerrain.h"
#include"CProtoMaker.h"
#include "UGpuCommand.h"
#include "CPlayer.h"
#include "UActor.h"
#include "UParticle.h"
#include "CTree.h"
#include "UVIBufferTerrain.h"
#include "CWater.h"
#include "CAimingPoint.h"
#include "CMonster.h"
#include "UAnimatedParticle.h"
#include "UFont.h"
#include "CBuilding.h"
#include  "UMirror.h"
#include "CStage2Scene.h"


CStage1Scene::CStage1Scene(CSHPTRREF<UDevice> _spDevice)
	: UScene(_spDevice, SCENE::SCENE_STAGE1),
	m_spMainCamera{ nullptr },
	m_spSkyBox{ nullptr },
	m_spTerrainObject{nullptr},
	m_spPlayer{nullptr},
	m_spParticle{nullptr}, 
	m_spBuilding{ nullptr },
	m_WinTimer{5.f}
{
}

void CStage1Scene::Free()
{
}

HRESULT CStage1Scene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	{
		SHPTR<UCommand> spCommand = static_pointer_cast<UCommand>(spGameInstance->GetGpuCommand());

		CProtoMaker::LoadStageScene1(spGameInstance, GetDevice(), spCommand);
	}
	{
		USkyBox::SKYBOXDESC tDesc{L"SkyBox", L"SkyBox_Cloud", true};
		VOIDDATAS vecDatas;
		{
			vecDatas.push_back(&tDesc);
		}
		m_spSkyBox = static_pointer_cast<USkyBox>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SKYBOX, vecDatas));
	}
	{
		{
			// Default로 CloneOnce니까 주의
			UTerrain::TERRAINDESC tDesc;
			tDesc.wstrBufferProtoName = PROTO_RES_VIBUFFERTERRAIN;
			tDesc.wstrDiffuseTexName = L"Detail_Texture_7";
			tDesc.wstrHeightTexName = L"height";
			tDesc.wstrTextureProtoName = PROTO_RES_TERRAINGROUP;
			// Terrain 생성
			VOIDDATAS vecDatas;
			{
				vecDatas.push_back(&tDesc);
			}

			m_spTerrainObject = std::static_pointer_cast<UTerrain>(spGameInstance->CloneActorAdd(PROTO_ACTOR_TERRAIN, vecDatas));
			m_spTerrainObject->GetTransform()->SetScale(_float3{ 100.f, 100.f, 100.f });
		//	m_spTerrainObject->GetTransform()->SetPos(_float3{ 0.f, -1000.f, 0.f });
		}
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
			vecDatas.push_back(&m_spTerrainObject);
		}
		m_spMainCamera = static_pointer_cast<CMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
		m_spMainCamera->GetTransform()->SetPos({ 0.f, 100.f,  0.f });
		m_spMainCamera->GetTransform()->LookAt({ 100.f, 0.f, 100.f });
	}
	// Tree Create
	{
		VOIDDATAS vecDatas;
		{
			vecDatas.push_back(&m_spTerrainObject);
			vecDatas.push_back(&m_spMainCamera);
		}
		m_spPlayer = static_pointer_cast<CPlayer>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PLAYER, vecDatas));
		m_spPlayer->GetTransform()->SetScale(_float3{ 50.f, 50.f, 50.f });
	}
	// Build Create
	{		
		SHPTR<UVIBufferTerrain> spBufferTerrain = m_spTerrainObject->GetVIBufferTerrain();
		m_spBuilding = static_pointer_cast<CBuilding>(spGameInstance->CloneActorAdd(PROTO_ACTOR_BUILDING));

		_float3 vPos = _float3{ 1000.f, 1000.f, 1000.f };
		spBufferTerrain->TerrainHeightPos(vPos, m_spTerrainObject->GetTransform()->GetWorldMatrix());
		vPos.y += 10.f;
		m_spBuilding->GetTransform()->SetPos(vPos);
	}
	{
		VOIDDATAS vecDatas;
		{
			vecDatas.push_back(&m_spTerrainObject);
			vecDatas.push_back(&m_spPlayer);
		}
		SHPTR<UVIBufferTerrain> spBufferTerrain = m_spTerrainObject->GetVIBufferTerrain();
	}
	{
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL, {1.f, 1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f, 0.05f}, {1.f, 1.f, 1.f, 1.f}, {0.f, -1.f, -1.f,},
			{0.f, 100.f, 0.f}, 0.f, 0.f, 1.f, 200.f }, 
			LIGHTCONTROL{RIMLIGHT{}});
	}
	{
		m_spScoreFont = spGameInstance->AddFont(FONT_NAMUSQAURE);
		m_spScoreFont->SetPos(_float2{ 0.f, 0.f });
		m_spScoreFont->SetText(L"Alive Enemy : ");
	}
	{
		m_spWinFont = spGameInstance->AddFont(FONT_NAMUSQAURE);
		m_spWinFont->SetPos(_float2{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	}
	m_WinTimer.ResetTimer();
	return S_OK;
}

void CStage1Scene::Tick(const _double& _dTimeDelta)
{
	if (true == m_isNextStage)
	{
	//	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	//	spGameInstance->RegisterScene(CreateConstructorNative<CStage2Scene>(GetDevice()));
	}
}

void CStage1Scene::LateTick(const _double& _dTimeDelta)
{
	if (true == m_spBuilding->Hit(m_spPlayer))
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		spGameInstance->RegisterScene(CreateConstructorNative<CStage2Scene>(GetDevice()));
	}
}

void CStage1Scene::CollisionTick(const _double& _dTimedelta)
{

}
