#include "ClientDefines.h"
#include "CLogoScene.h"
#include "UGameInstance.h"
#include "CMainScene.h"
#include "CImageUI.h"
#include "CButtonUI.h"
#include "CLoadingUI.h"

BEGIN(Client)

CLogoScene::CLogoScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice, SCENE_LOGO)
{
}

void CLogoScene::Free()
{
}

HRESULT CLogoScene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CImageUI::UIDESC tDesc;
	{
		tDesc.fZBufferOrder = 0.99f;
		tDesc.strImgName = L"Background";
		tDesc._shaderName = PROTO_RES_BACKGROUNDUISHADER;
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
		tDesc1.v2Size.x = static_cast<_float>(1080);
		tDesc1.v2Size.y = static_cast<_float>(30);
		tDesc1.v2Pos = _float2{ 640,840 };
		m_spLoadingBackgroundUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc1 }));
		m_spLoadingBackgroundUI->SetActive(false);
	}
	CButtonUI::UIDESC tDesc2;
	{
		tDesc2.fZBufferOrder = 0.97f;
		tDesc2.strImgName = L"LoadingBar_Fill";
		tDesc2._shaderName = PROTO_RES_LOADINGUISHADER;
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
		tDesc3.v2Size.x = static_cast<_float>(640);
		tDesc3.v2Size.y = static_cast<_float>(240);
		tDesc3.v2Pos = _float2{ 640,220 };
		m_spMainTitleUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc3 }));
		m_spMainTitleUI->SetActive(true);
	}
	CButtonUI::UIDESC tDesc4;
	{
		tDesc4.fZBufferOrder = 0.47f;
		tDesc4.strImgName = L"";
		tDesc4._shaderName = PROTO_RES_BUTTONUISHADER;
		tDesc4.v2Size.x = static_cast<_float>(200);
		tDesc4.v2Size.y = static_cast<_float>(100);
		tDesc4.v2Pos = _float2{ 640, 750 };
		m_spButtonUI = std::static_pointer_cast<CButtonUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_BUTTONUI, { &tDesc4 }));
		m_spButtonUI->SetActive(true);
	}
	CImageUI::UIDESC tDesc5;
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc5.fZBufferOrder = 0.9f;
		tDesc5.strImgName = L"T_TitleLogo_Shadow_SmokeWave_UI";
		tDesc5._shaderName = PROTO_RES_DEFAULTUISHADER;
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
		tDesc5.v2Size.x = static_cast<_float>(200);
		tDesc5.v2Size.y = static_cast<_float>(75);
		tDesc5.v2Pos = _float2{ 640,700 };
		m_spPleaseWaitTextUI = std::static_pointer_cast<CLoadingUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_LOADINGUI, { &tDesc5 }));
		m_spPleaseWaitTextUI->SetActive(false);
	}
	return S_OK;
}

void CLogoScene::Tick(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	
	if (m_spButtonUI->IsMouseOnRect()) {
		
		m_spButtonUI->SetIfPicked(true);
		
		if (true == spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_L)) {

			m_spBackgroundUI->SetActive(false);
			m_spMainTitleUI->SetActive(false);
			m_spLoadingTextUI->SetActive(false);
			m_spLineEffectUI->SetActive(false);
			m_spMainTitleEffectUI->SetActive(false);
			m_spLoadingDotsUI->SetActive(false);
			m_spPleaseWaitTextUI->SetActive(false);
			m_spLoadingFillingUI->SetActive(false);
			m_spLoadingBackgroundUI->SetActive(false);
			m_spButtonUI->SetActive(false);
			spGameInstance->RegisterScene(CreateConstructorNative<CMainScene>(GetDevice()));
		}
	}
	else {
		m_spButtonUI->SetIfPicked(false);
	}
		
	//spGameInstance->RegisterScene(CreateConstructorNative<CMainScene>(GetDevice()));
}

void CLogoScene::LateTick(const _double& _dTimeDelta)
{

	// 위치 변경
	//m_spOtherImageUI->UpdatePos(_float2{});

	// 사이즈 변경 변경
	// m_spOtherImageUI->UpdateSize(_float2{});
}

END