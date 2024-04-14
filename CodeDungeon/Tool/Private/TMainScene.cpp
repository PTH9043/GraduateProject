#include "ToolDefines.h"
#include "TMainScene.h"
#include "TMainCamera.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "UGrid.h"
#include "UVIBufferGrid.h"
#include "UTransform.h"
#include "UPicking.h"
#include "TGuizmoManager.h"
#include "ULight.h"

TMainScene::TMainScene(CSHPTRREF<UDevice> _spDevice)
	: UScene(_spDevice, (_uint)IMGTAG::MAIN),
	m_spMainCamera{ nullptr }
{
}

void TMainScene::Free()
{
}

HRESULT TMainScene::LoadSceneData()
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

		m_spMainCamera = std::static_pointer_cast<TMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
		m_spMainCamera->GetTransform()->SetPos({ 0.f, 10.f, -100.f });
	}
	{
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL,LIGHTVERSION::TYPE_ORIGINAL, {0.5f, 0.5f, 0.5f, 1.f}, {0.15f, 0.15f,0.15f, 1.f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.f, -1.f, 0.f,}, {0.f, 10000.f, 0.f}, 0.f, 0.f ,
			1.f, 200.f });

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_FLASHLIGHT,LIGHTVERSION::TYPE_ORIGINAL, {0.7f, 0.7f, 0.7f, 0.f}, {0.1f, 0.1f, 0.1f, 1.f}, {0.1f, 0.1f, 0.1f, 1.f}, {0.f, 0.f, 1.f,}
			, m_spMainCamera->GetTransform()->GetPos(), 150.f, 60.f ,
			100.f, 2.f, 8.0f,(float)cos(DirectX::XMConvertToRadians(30.f)),(float)cos(DirectX::XMConvertToRadians(15.f)),_float3(1.0f, 0.1f, 0.01f) });

		/*AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL, {1.f, 1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f, 0.05f}, {1.f, 1.f, 1.f, 1.f}, {0.f, -1.f, -1.f,},
			{0.f, 100.f, 0.f}, 0.f, 0.f, 1.f, 200.f },
			LIGHTCONTROL{ RIMLIGHT{} });*/
		
		
		/*AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT, {1.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 1.f,}, m_spMainCamera->GetTransform()->GetPos(), 0.f, 0.f ,
			1.f, 2.f });*/
		/*AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_FLASHLIGHT, {1.f, 0.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 1.f,}, {0.f, 50.f, -100.f}, 60.f, 60.f ,
			1.f, 2.f });*/
	}

	{
		_float GridScale = 7.0f;
		m_spTestObject = std::static_pointer_cast<UGrid>(spGameInstance->CloneActorAdd(PROTO_ACTOR_GRID));
		m_spTestObject->GetTransform()->SetScale({ GridScale, 1.0f, GridScale });
		m_spTestObject->GetTransform()->SetPos({ -(float)HALF_GRID_SIZE * GridScale, 0.f, -(float)HALF_GRID_SIZE * GridScale });
		
		MAINGRID stGrid;
		stGrid.spGrid = m_spTestObject;

		spGameInstance->AddPickingGrid(stGrid);

	}

	return S_OK;
}

void TMainScene::Tick(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<ULight> SpotLight;
	OutLight(LIGHTTYPE::TYPE_FLASHLIGHT, 0, SpotLight);
	SpotLight->SetLightPos(m_spMainCamera->GetTransform()->GetPos());
	SpotLight->SetDirection(m_spMainCamera->GetTransform()->GetLook());
	if (pGameInstance->GetDIKeyPressing(DIK_1))
		SpotLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	if (pGameInstance->GetDIKeyPressing(DIK_2))
		SpotLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);

	SHPTR<ULight> DirLight;
	OutLight(LIGHTTYPE::TYPE_DIRECTIONAL, 0, DirLight);
	if (pGameInstance->GetDIKeyPressing(DIK_3))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	if (pGameInstance->GetDIKeyPressing(DIK_4))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);
}

void TMainScene::LateTick(const _double& _dTimeDelta)
{

}
