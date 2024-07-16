#include "ClientDefines.h"
#include "CAnubis.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "URegion.h"
#include "UNavigation.h"
#include "CAnubis.h"
#include "CAnubisAnimController.h"
#include "UCell.h"
#include "UMethod.h"
#include "UPlayer.h"
#include "UCollider.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "CModelObjects.h"
#include "UAnimation.h"
#include "CAnubisStaff.h"

CAnubis::CAnubis(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType), m_AnubisType{},
	m_PathFindingState{},
	m_AstarPath{},
	m_isPathFinding{ false },
	m_currentPathIndex{ 0 },
	m_spAnubisStaff{ nullptr },
	m_f3OriginPos{ },
	m_f3OriginDirection{}
{
}

CAnubis::CAnubis(const CAnubis& _rhs)
	: CMob(_rhs), m_AnubisType{},
	m_PathFindingState{},
	m_AstarPath{},
	m_isPathFinding{ false },
	m_currentPathIndex{ 0 },
	m_spAnubisStaff{nullptr},
	m_f3OriginPos{},
	m_f3OriginDirection{}
{
}

void CAnubis::Free()
{
}

HRESULT CAnubis::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CAnubis::NativeConstructClone(const VOIDDATAS& _Datas)
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
		m_spBloodParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	{
		m_stParticleParam = m_spBloodParticle->GetParticleSystem()->GetParticleParam();
		m_stParticleType = m_spBloodParticle->GetParticleSystem()->GetParticleTypeParam();
		m_stParticleType->fParticleType = PARTICLE_TYPE_DEFAULT;
		m_stParticleType->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
		m_spBloodParticle->SetBloodTexture(0, L"blood"); // y값 증가 x 원
		m_spBloodParticle->SetBloodTexture(1, L"blood1"); // y값 증가 x 원
		m_spBloodParticle->SetBloodTexture(2, L"blood2"); // y값 증가 x 원
		m_spBloodParticle->SetBloodTexture(3, L"blood3"); // y값 증가 x 원
		m_spBloodParticle->SetBloodTexture(4, L"blood4"); // y값 증가 x 원
		m_spBloodParticle->SetBloodTexture(5, L"blood5"); // y값 증가 x 원
		m_spBloodParticle->SetBloodTexture(6, L"blood6"); // y값 증가 x 원
		m_spBloodParticle->SetBloodTexture(7, L"blood7"); // y값 증가 x 원
		m_spBloodParticle->SetParticleType(PARTICLE_BLOOD);
		*m_spBloodParticle->GetParticleSystem()->GetCreateInterval() = 0.85f;
		*m_spBloodParticle->GetParticleSystem()->GetAddParticleAmount() = 5;
	}
	{
		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTESLASH2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLESLASH2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 1.75f;
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 0.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 0.30;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.15f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 7;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 5;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 3;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.f, 0.f, 0.1f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticlePosition = _float3(0.f, 0.f, 0.f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleKind = PARTICLE_SLASH;
		m_spSlashParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	{
		m_spSlashParticle->GetParticleSystem()->GetParticleTypeParam()->fParticleType = PARTICLE_TYPE_DEFAULT;
		m_spSlashParticle->GetParticleSystem()->GetParticleTypeParam()->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
		m_spSlashParticle->SetTexture(L"Slash2");
		
		m_spSlashParticle->SetParticleType(PARTICLE_SLASH);
		*m_spSlashParticle->GetParticleSystem()->GetCreateInterval() = 0.35f;
		*m_spSlashParticle->GetParticleSystem()->GetAddParticleAmount() = 1;
	}
	{
		CAnubisStaff::EQDESC Desc1(std::static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_ANUBISSTAFFMODEL)), ThisShared<UCharacter>(), L"..\\..\\Resource\\Model\\Item\\Equip\\AnubisHook\\Convert\\EquipDesc\\Anubis_Staff_FBX.bin");
		m_spAnubisStaff = std::static_pointer_cast<CAnubisStaff>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ANUBISSTAFF, { &Desc1 }));

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
	SetActivationRange(50);
	SetDeactivationRange(80);

	return S_OK;
}

void CAnubis::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	GetAnimationController()->Tick(_dTimeDelta);
#ifndef _ENABLE_PROTOBUFF
	_float3 pos = GetTransform()->GetPos();
	
	pos.y += 5;
	m_spBloodParticle->SetPosition(pos);
	m_spSlashParticle->SetPosition(pos);
	_int CurAnimState = GetAnimationController()->GetAnimState();
	_float3 CurrentMobPos = GetTransform()->GetPos();
	_float3 CurrentPlayerPos = GetTargetPlayer()->GetTransform()->GetPos();
	SHPTR<UCell> CurrentMobCell = GetCurrentNavi()->GetCurCell();
	SHPTR<UCell> CurrentPlayerCell = GetTargetPlayer()->GetCurrentNavi()->GetCurCell();

	_float walkingSpeed = 15;

	if (CurAnimState == UAnimationController::ANIM_MOVE)
	{
		AddTimeAccumulator(_dTimeDelta);

		// A* for moving towards player when player is found
		if (GetFoundTargetState())
		{
			SetOutline(true);
			if (GetTimeAccumulator() >= 0.5)
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
				GetTransform()->SetDirectionFixedUp(-direction, _dTimeDelta, 5);
			}
			GetTransform()->TranslateDir(GetTransform()->GetLook(), _dTimeDelta, walkingSpeed);
		}
		else 
		{
			SetOutline(false);
			SHPTR<UNavigation> spNavigation = GetCurrentNavi();
			SHPTR<UCell> originCell = spNavigation->FindCell(m_f3OriginPos);
			if (GetTimeAccumulator() >= 0.5)
			{
				m_PathFindingState = (spNavigation->StartPathFinding(CurrentMobPos, originCell->GetCenterPos(), CurrentMobCell, originCell));
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
						m_AstarPath = (spNavigation->OptimizePath(m_PathFindingState.path, CurrentMobPos, originCell->GetCenterPos()));
						m_currentPathIndex = 0; // index initialized when path is optimized
					}
				}
			}
			if (!m_AstarPath.empty())
			{
				MoveAlongPath(m_AstarPath, m_currentPathIndex, _dTimeDelta);
				_float3 direction = CurrentMobPos - GetTargetPos();
				GetTransform()->SetDirectionFixedUp(-direction, _dTimeDelta, 5);
			}
			GetTransform()->TranslateDir(GetTransform()->GetLook(), _dTimeDelta, walkingSpeed);
		}
	}
	else if (CurAnimState == UAnimationController::ANIM_ATTACK)
	{
		_float3 direction = CurrentMobPos - CurrentPlayerPos;
		GetTransform()->SetDirectionFixedUp(-direction, _dTimeDelta, 5);
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
	}
	else if (CurAnimState == UAnimationController::ANIM_IDLE)
	{
		SetOutline(false);
		GetAnimModel()->TickAnimation(_dTimeDelta);
		GetTransform()->SetPos(GetTransform()->GetPos());
	}
	else
	{
		GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);
		SetElapsedTime(0.0);
	}

	UpdateCollision();
#else
GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);
#endif
}

void CAnubis::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);

	_float newHeight = GetCurrentNavi()->ComputeHeight(GetTransform()->GetPos());
	GetTransform()->SetPos(_float3(GetTransform()->GetPos().x, newHeight, GetTransform()->GetPos().z));

	//for (auto& Colliders : GetColliderContainer())
	//	if(Colliders.first == L"Main")
	//		Colliders.second->AddRenderer(RENDERID::RI_NONALPHA_LAST);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	_int CurAnimState = GetAnimationController()->GetAnimState();
	if (CurAnimState == UAnimationController::ANIM_DEATH)
	{
		if (GetElapsedTime() >= 100.0)
		{
			SetActive(false);
			spGameInstance->RemoveCollisionPawn(ThisShared<CMob>());
		}
	}
}

HRESULT CAnubis::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	const _wstring& CurAnimName = GetAnimModel()->GetCurrentAnimation()->GetAnimName();

	if(CurAnimName != L"staticLaying" && CurAnimName != L"staticStanding")
	{
		 __super::RenderActive(_spCommand, _spTableDescriptor);
	}
	return S_OK;
}

HRESULT CAnubis::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderShadowActive(_spCommand, _spTableDescriptor);
}

HRESULT CAnubis::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	return __super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
}

void CAnubis::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
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
					if (!GetIsHItAlreadyState())
					{
						m_spBloodParticle->SetActive(true);
						m_spSlashParticle->SetActive(true);
						m_spBloodParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
						m_spSlashParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
						// Decrease health on hit
						DecreaseHealth(pCharacter->GetAttack());
					}
					


					SetHitAlreadyState(true);
				}
			}
			else
			{
				SetHitAlreadyState(false);
			}

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
