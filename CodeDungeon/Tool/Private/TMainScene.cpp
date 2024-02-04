#include "ToolDefines.h"
#include "TMainScene.h"
#include "TMainCamera.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "TTestObject.h"
#include "UTransform.h"

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
		tDesc.stCamValue = UCamera::CAMVALUE(5.f, DirectX::XMConvertToRadians(90.f));
		tDesc.eCamType = CAMERATYPE::MAIN;
		// Actor Add Main Camera

		VOIDDATAS vecDatas;
		vecDatas.push_back(&tDesc);

		m_spMainCamera = std::static_pointer_cast<TMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
		m_spMainCamera->GetTransform()->SetPos({ 0.f, 0.f, -10.f });
	}
	{
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, -1.f, -1.f,}, {0.f, 10000.f, 0.f}, 0.f, 0.f ,
			1.f, 2.f });
	}

	//{
	//	m_spTestObject = std::static_pointer_cast<TTestObject>(spGameInstance->CloneActorAdd(L"Proto_Actor_TestObject"));
	//	m_spTestObject->GetTransform()->SetPos({ 0.f, 0.f, 0.f });
	//}

	return S_OK;
}

void TMainScene::Tick(const _double& _dTimeDelta)
{
}

void TMainScene::LateTick(const _double& _dTimeDelta)
{
}
