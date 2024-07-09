#include "ClientDefines.h"
#include "CLogoScene.h"
#include "UGameInstance.h"
#include "CMainScene.h"
#include "CImageUI.h"

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
		tDesc.fZBufferOrder = 0.1f;
		tDesc.strImgName = L"Background";
		tDesc.v2Size.x = static_cast<_float>(500);
		tDesc.v2Size.y = static_cast<_float>(500);
		tDesc.v2Pos = _float2{ (_float)(WINDOW_WIDTH), (_float)(WINDOW_HEIGHT) } / 2.f;
		m_spImageUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
	}
	{
		// ZBufferOrder는 이미지 Order 순서를 표현한다. 0에 가까울수록 맨 위, 1에 가까울수록 맨 뒤에 있는다. (0, 1)는 사용 X
		tDesc.fZBufferOrder = 0.2f;
		tDesc.strImgName = L"Icon_Oni";
		tDesc.v2Size.x = static_cast<_float>(400);
		tDesc.v2Size.y = static_cast<_float>(400);
		tDesc.v2Pos = _float2{ (_float)(WINDOW_WIDTH), (_float)(WINDOW_HEIGHT) } / 2.f;
		tDesc.v2Pos.x += 200.f;
		m_spOtherImageUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
	}
	return S_OK;
}

void CLogoScene::Tick(const _double& _dTimeDelta)
{
	if (m_spImageUI->IsMouseOnRect())
	{
		// Ratio를 변경하는 함수
		m_spOtherImageUI->UpdateRatio(0.05f);
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	//	if (true == spGameInstance->IsNetworkResourceRecvSuccess())

		//if(spGameInstance->IsP)
		{
		//	spGameInstance->RegisterScene(CreateConstructorNative<CMainScene>(GetDevice()));
		}
	}
	else
	{
		m_spOtherImageUI->UpdateRatio(0.2f);
	}
	// 다음으로 넘어가려면 아래 코드 해제
}

void CLogoScene::LateTick(const _double& _dTimeDelta)
{

	// 위치 변경
	//m_spOtherImageUI->UpdatePos(_float2{});

	// 사이즈 변경 변경
	// m_spOtherImageUI->UpdateSize(_float2{});
}

END