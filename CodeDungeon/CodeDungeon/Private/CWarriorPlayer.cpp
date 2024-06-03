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
#include "UParticle.h"
#include "UParticleSystem.h"
#include "CSword.h"

CWarriorPlayer::CWarriorPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPlayer(_spDevice, _wstrLayer, _eCloneType), m_spSword{nullptr}
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
	SHPTR<UCell> spCell = spNavigation->FindCell(133);

	GetTransform()->SetPos(spCell->GetCenterPos());

	SHPTR<CMainCamera> spMainCamera = std::static_pointer_cast<CMainCamera>(GetFollowCamera());
	spMainCamera->SetMoveState(false);
	GetAnimModel()->SetAnimation(L"idle01");
	//GetTransform()->RotateFix(_float3{ 0.f, DirectX::XMConvertToRadians(180.f), 0.f });
	GetTransform()->SetScale({ 0.5f, 0.5f, 0.5f });
	SetMovingSpeed(50.f);
	SetRunningSpeed(100.f);
	SetRunState(false);


	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	{

		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEFOOTPRINT2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLEFOOTPRINT2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle =7.5f;// 6.5f
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle =4.5f; //3.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 2.0f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 50;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = -GetTransform()->GetLook();
		tDesc.ParticleParam.stGlobalParticleInfo.fParticlePosition = GetTransform()->GetPos();
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleKind = PARTICLE_FOOTPRINT;
		m_spParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	m_stParticleParam = m_spParticle->GetParticleSystem()->GetParticleParam();
	m_stParticleType = m_spParticle->GetParticleSystem()->GetParticleTypeParam();
	m_stParticleType->fParticleType = PARTICLE_TYPE_AUTO;
	m_stParticleType->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
	m_spParticle->SetTexture(L"Sand"); //Dust3 ÂøÁö ¸ð¼Ç
	
	{
		*m_spParticle->GetParticleSystem()->GetCreateInterval() = 0.8f;
		*m_spParticle->GetParticleSystem()->GetAddParticleAmount() = 4;
		m_spParticle->SetParticleType(PARTICLE_FOOTPRINT);
		m_spParticle->SetActive(false);
	}

	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		CSword::EQDESC Desc(std::static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_LONGSWORDMODEL)), ThisShared<UCharacter>(), L"..\\..\\Resource\\Model\\Item\\Equip\\Sword\\Convert\\EquipDesc\\sword_FBX.bin");
		m_spSword = std::static_pointer_cast<CSword>(spGameInstance->CloneActorAdd(PROTO_ACTOR_LONGSWORD, {&Desc}));

	}
	return S_OK;
}

void CWarriorPlayer::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	GetAnimationController()->Tick(_dTimeDelta);
	GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);

	m_spParticle->SetActive(true);

	_int AnimState = GetAnimationController()->GetAnimState();

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	if (spGameInstance->GetDIKeyPressing(DIK_LSHIFT)&& spGameInstance->GetDIKeyPressing(DIK_W)) {//|| AnimState == CWarriorAnimController::ANIM_ATTACK|| AnimState == CWarriorAnimController::ANIM_COMBO
		*m_spParticle->GetParticleSystem()->GetAddParticleAmount() =4;
		*m_spParticle->GetParticleSystem()->GetCreateInterval() = 0.355f;
		_float3 pos = GetTransform()->GetPos() + GetTransform()->GetRight();
		pos.y += 1.6;

		_float3 Look = GetTransform()->GetLook();
		_float3 Right = 1.2 * GetTransform()->GetRight();
		//pos -= 3 * Look;
		m_spParticle->SetPosition(pos);
		m_spParticle->SetDirection(Right);
	}
	else {
		
		*m_spParticle->GetParticleSystem()->GetAddParticleAmount() = 0;
		*m_spParticle->GetParticleSystem()->GetCreateInterval() = 0.8f;
		//m_spParticle->SetActive(false);
	}
	// Rotation 
	{
		_long		MouseMove = 0;
		if (MouseMove = spGameInstance->GetDIMMoveState(DIMOUSEMOVE::DIMM_X))
		{
			GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), MouseMove * 5.f, _dTimeDelta);
		}
	}

	//// Move
	//if (CWarriorAnimController::ANIM_MOVE == AnimState || CWarriorAnimController::ANIM_JUMP_FRONT == AnimState)
	//{
	//	
	//	//TranslateStateMoveAndRunF(spGameInstance, _dTimeDelta, GetMovingSpeed());
	//	SetRunState(false);
	//}
	//

	//if (CWarriorAnimController::ANIM_RUN == AnimState || CWarriorAnimController::ANIM_JUMP_FRONT_RUN == AnimState)
	//{
	//	
	//	//TranslateStateMoveAndRunF(spGameInstance, _dTimeDelta, GetRunningSpeed());
	//	SetRunState(true);
	//}
	//

	//// Move
	//if (CWarriorAnimController::ANIM_MOVE == AnimState)
	//{
	//	//TranslateStateMoveAndRunF(spGameInstance, _dTimeDelta, GetMovingSpeed());
	//	SetRunState(false);
	//}

	//if (CWarriorAnimController::ANIM_RUN == AnimState)
	//{
	//	//TranslateStateMoveAndRunF(spGameInstance, _dTimeDelta, GetRunningSpeed());
	//	SetRunState(true);
	//}


	//if (CWarriorAnimController::ANIM_WALKBACK == AnimState)
	//{
	//	if (spGameInstance->GetDIKeyPressing(DIK_S))
	//	{
	//		//GetTransform()->MoveBack(_dTimeDelta, GetMovingSpeed());
	//		SetRunState(false);
	//	}
	//}

	//if (CWarriorAnimController::ANIM_RUNBACK == AnimState)
	//{
	//	if (spGameInstance->GetDIKeyPressing(DIK_S))
	//	{
	//		//GetTransform()->MoveBack(_dTimeDelta, GetRunningSpeed());
	//		SetRunState(true);
	//	}
	//}
	UpdateCollision();
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

