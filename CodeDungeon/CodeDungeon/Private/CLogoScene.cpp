#include "ClientDefines.h"
#include "CLogoScene.h"
#include "UGameInstance.h"
#include "CMainScene.h"
#include "CImageUI.h"
#include "CButtonUI.h"

BEGIN(Client)

CLogoScene::CLogoScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice, SCENE_LOGO), m_spImageUI{nullptr}
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
		tDesc.v2Size.x = static_cast<_float>(1280);
		tDesc.v2Size.y = static_cast<_float>(1080 );
		tDesc.v2Pos = _float2{ 640,540 };
		m_spImageUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
	}
	CButtonUI::UIDESC tDesc1;
	{
		tDesc1.fZBufferOrder = 0.1f;
		tDesc1.strImgName = L"";
		tDesc1.v2Size.x = static_cast<_float>(200);
		tDesc1.v2Size.y = static_cast<_float>(100);
		tDesc1.v2Pos = _float2{ (_float)(WINDOW_WIDTH), (_float)(WINDOW_HEIGHT)+400 } / 2.f;
		m_spButtonUI = std::static_pointer_cast<CButtonUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_BUTTONUI, { &tDesc1 }));
	}
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc.fZBufferOrder = 0.97f;
		tDesc.strImgName = L"MainTitle";
		tDesc.v2Size.x = static_cast<_float>(640);
		tDesc.v2Size.y = static_cast<_float>(240);
		tDesc.v2Pos = _float2{ 640,240};
		m_spMainTitleUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
	}
	//{
	//	// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
	//	tDesc.fZBufferOrder = 0.98f;
	//	tDesc.strImgName = L"T_PhotoFrame_Gradation_Black";
	//	tDesc.v2Size.x = static_cast<_float>(1280);
	//	tDesc.v2Size.y = static_cast<_float>(2160);
	//	tDesc.v2Pos = _float2{ 640,540 };
	//	m_spBackEffectUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
	//}
	return S_OK;
}

void CLogoScene::Tick(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	
	if (m_spButtonUI->IsMouseOnRect()) {
		
		m_spButtonUI->SetIfPicked(true);
		
		if (true == spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_L)) {

			m_spButtonUI->SetActive(false);
			m_spImageUI->SetActive(false);
			m_spMainTitleUI->SetActive(false);
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