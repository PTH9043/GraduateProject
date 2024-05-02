#include "ClientDefines.h"
#include "CWarriorPlayer.h"
#include "CWarriorAnimController.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"

CWarriorPlayer::CWarriorPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPlayer(_spDevice, _wstrLayer, _eCloneType)
{
}

CWarriorPlayer::CWarriorPlayer(const CWarriorPlayer& _rhs) : 
	UPlayer(_rhs)
{
}

void CWarriorPlayer::Free()
{
}

HRESULT CWarriorPlayer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CWarriorPlayer::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);

	SHPTR<CMainCamera> spMainCamera = std::static_pointer_cast<CMainCamera>(GetFollowCamera());
	spMainCamera->SetMoveState(false);
	GetTransform()->SetScale({ 0.1f, 0.1f, 0.1f });
	GetAnimModel()->SetAnimation(L"idle01");
	return S_OK;
}

void CWarriorPlayer::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);

	RETURN_CHECK(CWarriorAnimController::ANIM_IDLE == GetAnimationController()->GetAnimState(), ;);

	if (CWarriorAnimController::ANIM_MOVE == GetAnimationController()->GetAnimState())
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		if (spGameInstance->GetDIKeyPressing(DIK_W))
			GetTransform()->MoveForward(_dTimeDelta, 100);
		if (spGameInstance->GetDIKeyPressing(DIK_A))
			GetTransform()->MoveLeft(_dTimeDelta, 100);
		if (spGameInstance->GetDIKeyPressing(DIK_D))
			GetTransform()->MoveRight(_dTimeDelta, 100);
		if (spGameInstance->GetDIKeyPressing(DIK_S))
			GetTransform()->MoveBack(_dTimeDelta, 100);

		_long		MouseMove = 0;

		if (MouseMove = spGameInstance->GetDIMMoveState(DIMOUSEMOVE::DIMM_X))
			GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), MouseMove * 5.f, _dTimeDelta);

	}
}

void CWarriorPlayer::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
	__super::LateTickActive(_dTimeDelta);
}

HRESULT CWarriorPlayer::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CWarriorPlayer::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void CWarriorPlayer::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
