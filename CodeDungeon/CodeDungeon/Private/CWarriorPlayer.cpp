#include "ClientDefines.h"
#include "CWarriorPlayer.h"
#include "CWarriorAnimController.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "URegion.h"
#include "UNavigation.h"
#include "UCell.h"

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

	SHPTR<UNavigation> spNavigation = GetCurrentNavi();
	SHPTR<UCell> spCell = spNavigation->FindCell(703);

	GetTransform()->SetPos(spCell->GetCenterPos());

	SHPTR<CMainCamera> spMainCamera = std::static_pointer_cast<CMainCamera>(GetFollowCamera());
	spMainCamera->SetMoveState(false);
	GetAnimModel()->SetAnimation(L"idle01");
	//GetTransform()->RotateFix(_float3{ 0.f, DirectX::XMConvertToRadians(180.f), 0.f });
	GetTransform()->SetScale({ 0.5f, 0.5f, 0.5f });
	SetMovingSpeed(50.f);
	SetRunningSpeed(100.f);
	SetRunState(false);
	return S_OK;
}

void CWarriorPlayer::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	GetAnimationController()->Tick(_dTimeDelta);
	GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);

	_int AnimState = GetAnimationController()->GetAnimState();

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	// Rotation 
	{
		_long		MouseMove = 0;
		if (MouseMove = spGameInstance->GetDIMMoveState(DIMOUSEMOVE::DIMM_X))
		{
			GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), MouseMove * 5.f, _dTimeDelta);
		}
	}
	// Move
	if (CWarriorAnimController::ANIM_MOVE == AnimState)
	{
		TranslateStateMoveAndRunF(spGameInstance, _dTimeDelta, GetMovingSpeed());
		SetRunState(false);
	}

	if (CWarriorAnimController::ANIM_RUN == AnimState)
	{
		TranslateStateMoveAndRunF(spGameInstance, _dTimeDelta, GetRunningSpeed());
		SetRunState(true);
	}

	if (CWarriorAnimController::ANIM_WALKBACK == AnimState)
	{
		if (spGameInstance->GetDIKeyPressing(DIK_S))
		{
			GetTransform()->MoveBack(_dTimeDelta, GetMovingSpeed());
			SetRunState(false);
		}
	}

	if (CWarriorAnimController::ANIM_RUNBACK == AnimState)
	{
		if (spGameInstance->GetDIKeyPressing(DIK_S))
		{
			GetTransform()->MoveBack(_dTimeDelta, GetRunningSpeed());
			SetRunState(true);
		}
	}
}

void CWarriorPlayer::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
	__super::LateTickActive(_dTimeDelta);
	FollowCameraMove(_float3{0.f, 20.f, -40.f}, _dTimeDelta);
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

void CWarriorPlayer::TranslateStateMoveAndRunF(CSHPTRREF<UGameInstance> _spGameInstance, const _double& _dTimeDelta, const _float _fSpeed)
{
	if (_spGameInstance->GetDIKeyPressing(DIK_W))
	{
		GetTransform()->MoveForward(_dTimeDelta, _fSpeed);
	}
}

