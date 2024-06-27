#include "ClientDefines.h"
#include "CWarriorPlayer.h"
#include "CUserWarriorAnimController.h"
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
#include "UCollider.h"
#include "UTrail.h"
#include "UVIBufferTrail.h"
#include "CModelObjects.h"
#include "UProcessedData.h"


CWarriorPlayer::CWarriorPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPlayer(_spDevice, _wstrLayer, _eCloneType), 
	m_spSword{ nullptr }, 
	m_bisCollisionWithObj{ false }, 
	isAttack{ false }, 
	m_stParticleType{}, 
	m_stParticleParam{},
	m_spParticle{nullptr},
	m_spTrail{nullptr},
	m_f3CollidedNormal{}
{
}

CWarriorPlayer::CWarriorPlayer(const CWarriorPlayer& _rhs) : 
	UPlayer(_rhs), 
	m_spSword{ nullptr },
	m_bisCollisionWithObj{ false },
	isAttack{ false },
	m_stParticleType{},
	m_stParticleParam{},
	m_spParticle{ nullptr },
	m_spTrail{ nullptr },
	m_f3CollidedNormal{}
{
}

void CWarriorPlayer::Free()
{
#ifdef _ENABLE_PROTOBUFF



#endif
}

HRESULT CWarriorPlayer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CWarriorPlayer::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SHPTR<UNavigation> spNavigation = GetCurrentNavi();
	SHPTR<UCell> spCell = spNavigation->FindCell(741);

	GetTransform()->SetPos(spCell->GetCenterPos());

	SHPTR<CMainCamera> spMainCamera = std::static_pointer_cast<CMainCamera>(GetFollowCamera());
	if (nullptr != spMainCamera)
	{
		spMainCamera->SetMoveState(false);
	}
	GetAnimModel()->SetAnimation(L"idle01");
	//GetTransform()->RotateFix(_float3{ 0.f, DirectX::XMConvertToRadians(180.f), 0.f });
	GetTransform()->SetScale({ 0.5f, 0.5f, 0.5f });
	SetMovingSpeed(50.f);
	SetRunningSpeed(100.f);
	SetRunState(false);

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(0, 0, 0);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";

	AddColliderInContainer(mainColliderTag, Collider);
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
	m_spParticle->SetTexture(L"Sand");// DUST 
	
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
	{
		UVIBufferTrail::TrailVertexCnt tDesc;
		tDesc.iMaxVertexCount = 100;
		m_spTrail = std::static_pointer_cast<UTrail>(spGameInstance->CloneActorAdd(PROTO_ACTOR_TRAIL, { &tDesc }));
		m_spTrail->SetActive(true);
		m_spTrail->SetColor(_float4(65.f / 255.f, 150.f / 255.f, 43.f / 255.f, 0.f));
	}

	for (auto& Colliders : GetColliderContainer())
	{
		Colliders.second->SetScale(_float3(3, 15, 3));
	}
	return S_OK;
}

void CWarriorPlayer::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
{
#ifdef _ENABLE_PROTOBUFF

	switch (_ProcessData.GetDataType())
	{
	case TAG_SC_PLAYERSTATE:
	{
		SC_PLAYERSTATE pPlayerData;
		pPlayerData.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());

		IfAttack(pPlayerData.ifattack());
		SetJumpingState(pPlayerData.jumpingstate());

		GetAnimationController()->ReceiveNetworkProcessData(&pPlayerData);
	}
	break;
	}
#endif
}

void CWarriorPlayer::TickActive(const _double& _dTimeDelta)
{

	__super::TickActive(_dTimeDelta);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (true == spGameInstance->IsMouseInWindowSize())
	{
		GetAnimationController()->Tick(_dTimeDelta);
		_long		MouseMove = GetMouseMove();
		if (MouseMove)
		{
			GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), MouseMove * 5.f, _dTimeDelta);
		}
	}
	


	_int AnimState = GetAnimationController()->GetAnimState();
	SHPTR<UCollider> ps = GetAnimModel()->BringAttackCollider(UCollider::TYPE_OBB);

	if (ps) {
		SHPTR<DirectX::BoundingOrientedBox> OBB = ps->GetOBB();

		_float3 plusPoint = ps->GetHeightAdjustedPointFromCenter(OBB, false);
		_float3 minusPoint = ps->GetHeightAdjustedPointFromCenter(OBB, true);

		_float4x4 AnimTransform = ps->GetTransformMatrix();


		m_spTrail->SetRenderingTrail(isAttack);


		m_spTrail->AddTrail(plusPoint, minusPoint);
	}
	
	
	GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);


	m_spParticle->SetActive(true);

	if (GetAnimationController()->GetAnimState() == CUserWarriorAnimController::ANIM_RUN)  {//|| AnimState == CWarriorAnimController::ANIM_ATTACK|| AnimState == CWarriorAnimController::ANIM_COMBO
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
	}
	// Rotation 
	{
		POINT ptCursorPos;
	//	ShowCursor(FALSE);
	//SetCursorPos(1000, 400);
	}
	UpdateCollision();
}

void CWarriorPlayer::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
	_float3 direction(0.0f, 0.0f, 0.0f);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (GetCollisionState())
	{
//		GetTransform()->SetPos(GetPrevPos());
	}
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
	FollowCameraMove(_float3{ 0.f, 20.f, -40.f }, _dTimeDelta);
}


HRESULT CWarriorPlayer::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CWarriorPlayer::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderShadowActive(_spCommand, _spTableDescriptor);
}
HRESULT CWarriorPlayer::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor,_bool _pass)
{
	return __super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
}

void CWarriorPlayer::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	PAWNTYPE ePawnType = _pEnemy->GetPawnType();

	if (PAWNTYPE::PAWN_CHAR == ePawnType)
	{
		UCharacter* pCharacter = static_cast<UCharacter*>(_pEnemy.get());
		for (auto& iter : GetColliderContainer())
		{
			if (pCharacter->GetAnimModel()->IsCollisionAttackCollider(iter.second))
			{
				SetHitstate(true);
			}
			else
				SetHitstate(false);

			GetTransform()->SetPos(GetTransform()->GetPos() - GetTransform()->GetLook() * 5 * _dTimeDelta);
		}
	}
	else if (PAWNTYPE::PAWN_STATICOBJ == ePawnType)
	{
		CModelObjects* pModelObject = static_cast<CModelObjects*>(_pEnemy.get());
		for (auto& iter : GetColliderContainer())
		{
			for (auto& iter2 : pModelObject->GetColliderContainer())
			{
				m_f3CollidedNormal = iter.second->GetOBBCollisionNormal(iter2.second);

				if (m_f3CollidedNormal != _float3::Zero) // 충돌이 발생한 경우
				{
					_float3 currentPosition = GetTransform()->GetPos();
					_float3 movementDirection = currentPosition - GetPrevPos();
					float dotProduct = DirectX::XMVector3Dot(XMLoadFloat3(&movementDirection), XMLoadFloat3(&m_f3CollidedNormal)).m128_f32[0];
					_float3 slidingVector = movementDirection - m_f3CollidedNormal * dotProduct;

					// 속도 결정
					_float speed = spGameInstance->GetDIKeyPressing(DIK_LSHIFT) ? 50.0f : 10.0f;

					// 충돌 보정 및 슬라이딩 벡터 적용
					_float3 newPosition = GetPrevPos() + slidingVector * speed * _dTimeDelta;

					// 위치 업데이트
					GetTransform()->SetPos(newPosition);
				}
			}
		}
	}
}

void CWarriorPlayer::TranslateStateMoveAndRunF(CSHPTRREF<UGameInstance> _spGameInstance, const _double& _dTimeDelta, const _float _fSpeed)
{
	if (_spGameInstance->GetDIKeyPressing(DIK_W))
	{
		GetTransform()->MoveForward(_dTimeDelta, _fSpeed);
	}
}

