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
		m_spMainCamera->GetTransform()->SetPos({ 0, 0, 0 });
	}
	{
		
		
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL,LIGHTACTIVE::ISACTIVE, {1.f, 1.f, 1.f, 1.f}, {0.2f, 0.2f,0.2f, 1.f}, {0.15f, 0.15f, 0.15f, 1.f}, {0.f, -1.f, 0.f,}, {0.f, 100.f, 0.f}, 0.f, 0.f ,
			1.f, 20.f });
		
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.8f, 0.4f, 0.16f, 1.f}, {0.f, 0.f, 1.f,},
		_float3(-883.653748, -145.2615, 847.1639), 60.f, 0.f ,
		1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });
		/*
		* ���� ��Ƽ������ Diffuse�� ���� ȸ������ �����ְ� ���� ���ص� ����. Ambient, Specular�������� ��κ� �������̶�, ������ Ambient Specular�� ���ϸ� ���õǴ� �����̶� ������ϴ°� ���ƺ���.
		* 
		1. Directional Light
		���� : �ʿ����� vDiffuse , vAmbient, vSpecular, fLightPower,fSpecularPowValue
		��� : �ʿ����� vDiffuse, vAmbient, vSpecular
		
		2.SpotLight
		����: �ʿ����� vDiffuse , vAmbient, vSpecular,vDirection, vPosition, fLightPower, fRange,fAngle, fLightPower, fSpecularPowValue
		���: �ʿ����� vDiffuse , vAmbient, vSpecular,vDirection, vPosition, fRange,fSpecularPowValue, fFallOff,fPhi,fTheta, ������Ʈ ����(vAttenuation ���ʿ�) ������: �ʿ�

		2.PointLight
		����: �ʿ����� vDiffuse , vAmbient, vSpecular,vDirection, vPosition, fLightPower, fRange,fAngle, fLightPower, fSpecularPowValue
		���: �ʿ����� vDiffuse , vAmbient, vSpecular, vPosition, fRange, vAttenuation 

		*/
		//���� ��� �������̳� ������ �������̳� Directional Light�� Material�����ص�, Directional�� Ambient�� �״�� ȥ��, ����� Specular�� Material�� 0���� ����� 0���� ����Ǿ�
		// �����̴� Specular 0�� ���Ͽ� �ᱹ �Ȱ��� ���´�. ������� LightPower�������ȵǾ��ִ�. ���� ����� ���� ����������� ����ϸ�, �����̴� ��������� ����Ѵ�.

		//Diffuse Light�� Ȯ���������� ���� ��ü�� �Ų����� ���� ǥ��, ��ģ ǥ�鿡�� �Ͼ.
		//Ȯ�� ǥ���� ��ü�� ǥ�鿡�� ��� �������� �����ϰ� ���� �ݻ��Ѵٰ� ����.

	/*	AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_SPOT,LIGHTACTIVE::ISACTIVE, {0.75f,0.75f,0.75f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.15f, 0.125f, 0.11f, 1.f}, {0.f, -1.f, 0.f,},
		_float3(-355,-20,166), 100.f, 0.f ,
		1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });*/
		
		/*	AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_SPOT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 0.f}, {0.15f, 0.125f, 0.11f, 1.f}, {1.f, 0.5f, 0.2f, 1.f}, {0.f, -1.f, 0.f,}
			, m_spMainCamera->GetTransform()->GetPos(), 100.f, 60.f ,
			100.f, 32.f, 8.0f,(float)cos(DirectX::XMConvertToRadians(30.f)),(float)cos(DirectX::XMConvertToRadians(15.f)),_float3(1.0f, 0.01f, 0.0001f) });*/
		
		/*
		* SpecularPowValue�� Ŭ���� ���ҹ��� �پ�� �� ������������ �ݻ�. ���� 32 . 1�ΰ����� Specular���� Ŀ��.
		�ڿ������� ����:

		Ambient: 0.2 ~ 0.4 ������ ��. �� ���� ��� ��ü�� ���� ������ �����մϴ�.
		Diffuse: 0.6 ~ 0.8 ������ ��. �� ���� �������� ������ ������ �����մϴ�.
		Specular: 0.3 ~ 0.5 ������ ��. �� ���� ���̶���Ʈ�� ������ �����մϴ�.
		������ ����:
		
		Ambient: 0.1 ~ 0.3 ������ ��. ���� ������ ������ �� �������̰� ��� �ְ� ����ϴ�.
		Diffuse: 0.8 ~ 1.0 ������ ��. ���� ������ ǥ�鿡 ���������� ��� ���� ������ ���Դϴ�.
		Specular: 0.5 ~ 0.7 ������ ��. ���� ������ ���̶���Ʈ�� �����մϴ�.
		�ε巯�� ����:
		
		Ambient: 0.4 ~ 0.6 ������ ��. ���� ������ ������ �ε巴�� ����ϴ�.
		Diffuse: 0.5 ~ 0.7 ������ ��. �߰� ������ ������ ������ ������ �����ϰ� �����մϴ�.
		Specular: 0.2 ~ 0.4 ������ ��. ���� ������ ���̶���Ʈ�� �ε巴�� �����մϴ�.
		*/
	
		
		
		/*AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.8f, 0.4f, 0.16f, 1.f}, {0.f, 0.f, 1.f,},
			_float3(-555.183f,-32.f,149.312f), 40.f, 0.f ,
			1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f)});

		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 1.f}, {0.4f, 0.2f, 0.08f, 1.f}, {0.15f, 0.125f, 0.11f, 1.f}, {0.f, 0.f, 1.f,},
		_float3(50,0,0), 40.f, 0.f ,
		1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });*/
		
	/*	AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_POINT,LIGHTACTIVE::ISACTIVE, {0.3f, 0.3f, 0.3f, 1.f}, {0.2f, 0.1f, 0.04f, 1.f}, {0.15f, 0.125f, 0.11f, 1.f}, {0.f, 0.f, 1.f,},
			_float3(0,0,0), 20.f, 0.f ,
			1.f, 32.f,0.f,0.f,0.f,_float3(1.f,0.01f,0.0001f) });*/
		
		/*AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_FLASHLIGHT, {1.f, 0.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 1.f,}, {0.f, 50.f, -100.f}, 60.f, 60.f ,
			1.f, 2.f });*/
	}

	{
		_float GridScale = 7.0f;
		m_spGrid = std::static_pointer_cast<UGrid>(spGameInstance->CloneActorAdd(PROTO_ACTOR_GRID));
		m_spGrid->GetTransform()->SetScale({ GridScale, 1.0f, GridScale });
		m_spGrid->GetTransform()->SetPos({ -(float)HALF_GRID_SIZE * GridScale, 0.f, -(float)HALF_GRID_SIZE * GridScale });
	}

	return S_OK;
}

void TMainScene::Tick(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<ULight> SpotLight;
	/*OutLight(LIGHTTYPE::TYPE_SPOT, 0, SpotLight);
	SpotLight->SetLightPos(m_spMainCamera->GetTransform()->GetPos());*/
//	SpotLight->SetDirection(m_spMainCamera->GetTransform()->GetLook());


	/*SHPTR<ULight> DirLight;
	OutLight(LIGHTTYPE::TYPE_DIRECTIONAL, 0, DirLight);
	if (pGameInstance->GetDIKeyPressing(DIK_4))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	if (pGameInstance->GetDIKeyPressing(DIK_5))
		DirLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);


	SHPTR<ULight> PointLight;
	OutLight(LIGHTTYPE::TYPE_POINT, 0, PointLight);
	if (pGameInstance->GetDIKeyPressing(DIK_6))
		PointLight->SetLightVersion(LIGHTVERSION::TYPE_ORIGINAL);
	if (pGameInstance->GetDIKeyPressing(DIK_7))
		PointLight->SetLightVersion(LIGHTVERSION::TYPE_YONGBBA);
	_float3 pos = m_spMainCamera->GetTransform()->GetPos();
	_float3 Look = m_spMainCamera->GetTransform()->GetLook();
	PointLight->SetLightPos(_float3(-546, -32, 131));*/

	/*SHPTR<ULight> PointLight2;
	OutLight(LIGHTTYPE::TYPE_POINT, 1, PointLight2);
	PointLight2->SetLightPos(_float3(-360, -20,253));*/

}

void TMainScene::LateTick(const _double& _dTimeDelta)
{

}
