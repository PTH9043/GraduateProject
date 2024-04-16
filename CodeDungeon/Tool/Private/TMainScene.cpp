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
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL,LIGHTVERSION::TYPE_ORIGINAL, {0.5f, 0.5f, 0.5f, 1.f}, {0.15f, 0.15f,0.15f, 1.f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.f, -1.f, 0.f,}, {0.f, 100.f, 0.f}, 0.f, 0.f ,
			1.f, 20.f });

		//현재 용빠 라이팅이나 태현이 라이팅이나 Directional Light는 Material적용해도, Directional은 Ambient를 그대로 혼합, 용빠는 Specular는 Material이 0으로 결과가 0으로 추출되어
		// 태현이는 Specular 0을 곱하여 결국 똑같이 나온다. 용빠꺼는 LightPower적용은안되어있다. 또한 용빠는 현재 월드공간에서 계산하며, 태현이는 뷰공간에서 계산한다.

		//Diffuse Light는 확산조명으로 보통 물체의 매끄럽지 않은 표면, 거친 표면에서 일어남.
		//확산 표면은 물체의 표면에서 모든 방향으로 균일하게 빛을 반사한다고 가정.
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_SPOT,LIGHTVERSION::TYPE_YONGBBA, {0.3f, 0.3f, 0.3f, 0.f}, {0.2f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 1.f,}
			, m_spMainCamera->GetTransform()->GetPos(), 100.f, 60.f ,
			100.f, 1.f, 8.0f,(float)cos(DirectX::XMConvertToRadians(30.f)),(float)cos(DirectX::XMConvertToRadians(15.f)),_float3(1.0f, 0.01f, 0.0001f) });

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
	OutLight(LIGHTTYPE::TYPE_SPOT, 0, SpotLight);
	SpotLight->SetLightPos(m_spMainCamera->GetTransform()->GetPos());
	SpotLight->SetDirection(m_spMainCamera->GetTransform()->GetLook());
	if (pGameInstance->GetDIKeyPressing(DIK_1))
		SpotLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	if (pGameInstance->GetDIKeyPressing(DIK_2))
		SpotLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);
	if (pGameInstance->GetDIKeyPressing(DIK_3))
		SpotLight->SetLightVersion(LIGHTVERSION::TYPE_END);

	SHPTR<ULight> DirLight;
	OutLight(LIGHTTYPE::TYPE_DIRECTIONAL, 0, DirLight);
	if (pGameInstance->GetDIKeyPressing(DIK_4))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	if (pGameInstance->GetDIKeyPressing(DIK_5))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);
}

void TMainScene::LateTick(const _double& _dTimeDelta)
{

}
