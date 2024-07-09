#include "ClientDefines.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "URegion.h"
#include "UNavigation.h"
#include "CMummyAnimController.h"
#include "UCell.h"
#include "UMethod.h"
#include "UPlayer.h"
#include "UCollider.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "CModelObjects.h"
#include "UAnimation.h"

CMummy::CMummy(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType), m_MummyType{},
	m_PathFindingState{},
	m_AstarPath{},
	m_isPathFinding{ false },
	m_currentPathIndex{ 0 }
{
}

CMummy::CMummy(const CMummy& _rhs)
	: CMob(_rhs), m_MummyType{},
	m_PathFindingState{},
	m_AstarPath{},
	m_isPathFinding{ false },
	m_currentPathIndex{ 0 }
{
}

void CMummy::Free()
{
}

HRESULT CMummy::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CMummy::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	{
		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEBLOODEFFECT2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLEBLOOD2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 3.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 0.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 1.;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.3f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 7;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 5;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 400;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.f, 0.f, 0.1f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticlePosition = _float3(0.f, 0.f, 0.f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleKind = PARTICLE_BLOOD;
		m_spParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	{
		m_stParticleParam = m_spParticle->GetParticleSystem()->GetParticleParam();
		m_stParticleType = m_spParticle->GetParticleSystem()->GetParticleTypeParam();
		m_stParticleType->fParticleType = PARTICLE_TYPE_DEFAULT;
		m_stParticleType->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
		m_spParticle->SetBloodTexture(0, L"blood"); // y값 증가 x 원
		m_spParticle->SetBloodTexture(1, L"blood1"); // y값 증가 x 원
		m_spParticle->SetBloodTexture(2, L"blood2"); // y값 증가 x 원
		m_spParticle->SetBloodTexture(3, L"blood3"); // y값 증가 x 원
		m_spParticle->SetBloodTexture(4, L"blood4"); // y값 증가 x 원
		m_spParticle->SetBloodTexture(5, L"blood5"); // y값 증가 x 원
		m_spParticle->SetBloodTexture(6, L"blood6"); // y값 증가 x 원
		m_spParticle->SetBloodTexture(7, L"blood7"); // y값 증가 x 원
		m_spParticle->SetParticleType(PARTICLE_BLOOD);
		*m_spParticle->GetParticleSystem()->GetCreateInterval() = 0.85f;
		*m_spParticle->GetParticleSystem()->GetAddParticleAmount() = 5;
	}


	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(0, 0, 0);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";

	AddColliderInContainer(mainColliderTag, Collider);
	for (auto& Colliders : GetColliderContainer())
	{
		Colliders.second->SetScale(_float3(1, 8, 1));
		Colliders.second->SetTranslate(_float3(0, 10, 0));
	}

	SetHealth(100);

	return S_OK;
}

void CMummy::TickActive(const _double& _dTimeDelta)
{
	_float3 pos = GetTransform()->GetPos();
	pos.y += 5;
	m_spParticle->SetPosition(pos);

	__super::TickActive(_dTimeDelta);
	GetAnimationController()->Tick(_dTimeDelta);
	_int CurAnimState = GetAnimationController()->GetAnimState();
	_float3 CurrentMobPos = GetTransform()->GetPos();
	_float3 CurrentPlayerPos = GetTargetPlayer()->GetTransform()->GetPos();
	SHPTR<UCell> CurrentMobCell = GetCurrentNavi()->GetCurCell();
	SHPTR<UCell> CurrentPlayerCell = GetTargetPlayer()->GetCurrentNavi()->GetCurCell();

	if (CurAnimState == UAnimationController::ANIM_MOVE)
	{
		AddTimeAccumulator(_dTimeDelta);

		// A* for moving towards player when player is found
		if (GetFoundTargetState())
		{
			SetOutline(true);
			if (GetTimeAccumulator() >= 1.0)
			{
				SHPTR<UNavigation> spNavigation = GetCurrentNavi();
				m_PathFindingState = (spNavigation->StartPathFinding(CurrentMobPos, CurrentPlayerPos, CurrentMobCell, CurrentPlayerCell));
				m_isPathFinding = true;
				SetTimeAccumulator(0.0);
			}
			if (m_isPathFinding)
			{
				SHPTR<UNavigation> spNavigation = GetCurrentNavi();
				if (spNavigation->StepPathFinding(m_PathFindingState))
				{
					m_isPathFinding = false;
					if (m_PathFindingState.pathFound)
					{
						m_AstarPath = (spNavigation->OptimizePath(m_PathFindingState.path, CurrentMobPos, CurrentPlayerPos));
						m_currentPathIndex = 0; // index initialized when path is optimized
					}
				}
			}
			if (!m_AstarPath.empty())
			{			
				MoveAlongPath(m_AstarPath, m_currentPathIndex, _dTimeDelta);
				_float3 direction = CurrentMobPos - GetTargetPos();
				GetTransform()->SetDirectionFixedUp(direction, _dTimeDelta, 5);
			}
		}
		else // patrolling when player is not found
		{
			SetOutline(false);
			SHPTR<UNavigation> spNavigation = GetCurrentNavi();
			SHPTR<UCell> spNeighborCell = spNavigation->ChooseRandomNeighborCell(3);
			if (GetTimeAccumulator() >= 5.0)
			{
				m_PathFindingState = (spNavigation->StartPathFinding(CurrentMobPos, spNeighborCell->GetCenterPos(), CurrentMobCell, spNeighborCell));
				m_isPathFinding = true;
				SetTimeAccumulator(0.0);
			}
			if (m_isPathFinding)
			{
				if (spNavigation->StepPathFinding(m_PathFindingState))
				{
					m_isPathFinding = false;
					if (m_PathFindingState.pathFound)
					{
						m_AstarPath = (spNavigation->OptimizePath(m_PathFindingState.path, CurrentMobPos, spNeighborCell->GetCenterPos()));
						m_currentPathIndex = 0; // index initialized when path is optimized
					}
				}
			}
			if (!m_AstarPath.empty())
			{
				MoveAlongPath(m_AstarPath, m_currentPathIndex, _dTimeDelta);
				_float3 direction = CurrentMobPos - GetTargetPos();
				GetTransform()->SetDirectionFixedUp(direction, _dTimeDelta, 5);
			}
		}
	}
	else if (CurAnimState == UAnimationController::ANIM_ATTACK)
	{
		_float3 direction = CurrentMobPos - CurrentPlayerPos;
		GetTransform()->SetDirectionFixedUp(direction, _dTimeDelta, 5);
	}
	else if (CurAnimState == UAnimationController::ANIM_HIT)
	{
		GetTransform()->TranslateDir(GetTransform()->GetLook(), _dTimeDelta, 1);
	}

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	// death animation
	if (CurAnimState == UAnimationController::ANIM_DEATH)
	{
		_double DeathAnimSpeed = 20;
		SetElapsedTime(GetElapsedTime() + (_dTimeDelta * DeathAnimSpeed));
		_double DeathTimeArcOpenEnd = 50;
		if (GetElapsedTime() < DeathTimeArcOpenEnd)
			GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, GetElapsedTime());

		if (GetElapsedTime() >= 100.0)
		{
			SetActive(false);
			spGameInstance->RemoveCollisionPawn(ThisShared<CMob>());
		}
	}
	else if (CurAnimState == UAnimationController::ANIM_IDLE)
	{
		GetAnimModel()->TickAnimation(_dTimeDelta);
		GetTransform()->SetPos(GetTransform()->GetPos());
	}
	else
	{
		GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);
		SetElapsedTime(0.0);
	}

	UpdateCollision();
}

void CMummy::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
	//for (auto& Colliders : GetColliderContainer())
	//	if(Colliders.first == L"Main")
	//		Colliders.second->AddRenderer(RENDERID::RI_NONALPHA_LAST);
}

HRESULT CMummy::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	const _wstring& CurAnimName = GetAnimModel()->GetCurrentAnimation()->GetAnimName();

	if(CurAnimName != L"staticLaying" && CurAnimName != L"staticStanding")
	{
		return __super::RenderActive(_spCommand, _spTableDescriptor);
	}
}

HRESULT CMummy::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderShadowActive(_spCommand, _spTableDescriptor);
}

HRESULT CMummy::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	return __super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
}

void CMummy::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	PAWNTYPE ePawnType = _pEnemy->GetPawnType();
	const _wstring& CurAnimName = GetAnimModel()->GetCurrentAnimation()->GetAnimName();

	_float3 direction = (_pEnemy->GetTransform()->GetPos() - GetTransform()->GetPos());
	direction.Normalize();

	auto handleCollisionWithPlayer = [&](UCharacter* pCharacter) {
		for (const auto& iter : GetColliderContainer())
		{
			if (pCharacter->GetAnimModel()->IsCollisionAttackCollider(iter.second))
			{
				if (CurAnimName != L"openLaying" && CurAnimName != L"openStanding" &&
					CurAnimName != L"taunt" && CurAnimName != L"death")
				{
				    SetHitstate(true);
					m_spParticle->SetActive(true);
					m_spParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
					// Decrease health on hit
					DecreaseHealth(pCharacter->GetAttack());				
				}
			}

			for (const auto& iter2 : pCharacter->GetColliderContainer())
			{
				if (iter.second->IsCollision(iter2.second))
				{
					SetCollisionState(true);
					GetTransform()->SetPos(GetTransform()->GetPos() + GetTransform()->GetLook() * 10 * _dTimeDelta);
				}
				else
				{
					SetCollisionState(false);
				}
			}
		}
		};

	auto handleCollisionWithCharacter = [&](UCharacter* pCharacter) {
		for (const auto& iter : GetColliderContainer())
		{
			for (const auto& iter2 : pCharacter->GetColliderContainer())
			{
				if (iter.second->IsCollision(iter2.second))
				{
					SetCollisionState(true);
					GetTransform()->SetPos(GetTransform()->GetPos() - direction * 7 * _dTimeDelta);
				}
				else
				{
					SetCollisionState(false);
				}
			}
		}
		};

	auto handleCollisionWithStaticObject = [&](CModelObjects* pModelObject) {
		for (const auto& iter : GetColliderContainer())
		{
			for (const auto& iter2 : pModelObject->GetColliderContainer())
			{
				SetCollidedNormal(iter.second->GetCollisionNormal(iter2.second));
				if (GetCollidedNormal() != _float3::Zero)
				{
					_float speed = spGameInstance->GetDIKeyPressing(DIK_LSHIFT) ? 50.0f : 20.0f;
					ApplySlidingMovement(GetCollidedNormal(), speed, _dTimeDelta);
				}
			}
		}
		};

	if (ePawnType == PAWNTYPE::PAWN_PLAYER)
	{
		UCharacter* pCharacter = static_cast<UCharacter*>(_pEnemy.get());
		handleCollisionWithPlayer(pCharacter);
	}
	else if (ePawnType == PAWNTYPE::PAWN_CHAR)
	{
		UCharacter* pCharacter = static_cast<UCharacter*>(_pEnemy.get());
		handleCollisionWithCharacter(pCharacter);
	}
	else if (ePawnType == PAWNTYPE::PAWN_STATICOBJ)
	{
		CModelObjects* pModelObject = static_cast<CModelObjects*>(_pEnemy.get());
		handleCollisionWithStaticObject(pModelObject);
	}
}
