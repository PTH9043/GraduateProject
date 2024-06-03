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

CMummy::CMummy(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType), m_MummyType{}
{
}

CMummy::CMummy(const CMummy& _rhs)
	: CMob(_rhs), m_MummyType{}
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
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 300;
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
		*m_spParticle->GetParticleSystem()->GetAddParticleAmount() = 20;
	}


	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(0, 0, 0);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";

	AddColliderInContainer(mainColliderTag, Collider);


	return S_OK;
}

void CMummy::TickActive(const _double& _dTimeDelta)
{

	
	_float3 pos = GetTransform()->GetPos();
	pos.y +=3.5;
	m_spParticle->SetPosition(pos);

	__super::TickActive(_dTimeDelta);
	GetAnimationController()->Tick(_dTimeDelta);
	_int CurAnimState = GetAnimationController()->GetAnimState();
	_float3 CurrentMobPos = GetTransform()->GetPos();
	_float3 CurrentPlayerPos = GetTargetPlayer()->GetTransform()->GetPos();
	SHPTR<UCell> CurrentMobCell = GetCurrentNavi()->GetCurCell();
	SHPTR<UCell> CurrentPlayerCell = GetTargetPlayer()->GetCurrentNavi()->GetCurCell();

	static _double timeAccumulator = 0.0;
	static UNavigation::PathFindingState pathFindingState;
	static bool isPathFinding = false;
	static VECTOR<_float3> path;
	static size_t currentPathIndex = 0;

	if(CurAnimState == UAnimationController::ANIM_MOVE)
	{
		
		timeAccumulator += _dTimeDelta;
		if (GetFoundTargetState() && timeAccumulator >= 2.0)
		{
			SHPTR<UNavigation> spNavigation = GetCurrentNavi();
			pathFindingState = spNavigation->StartPathFinding(CurrentMobPos, CurrentPlayerPos, CurrentMobCell, CurrentPlayerCell);
			isPathFinding = true;
			timeAccumulator = 0.0;
		}
		if (isPathFinding)
		{
			SHPTR<UNavigation> spNavigation = GetCurrentNavi();
			if (spNavigation->StepPathFinding(pathFindingState))
			{
				isPathFinding = false;
				if (pathFindingState.pathFound)
				{
					path = spNavigation->OptimizePath(pathFindingState.path, CurrentMobPos, CurrentPlayerPos);
					currentPathIndex = 0; // 경로를 재설정하면 인덱스를 초기화합니다
				}
			}
		}
		if (GetFoundTargetState() && !path.empty())
		{
			MoveAlongPath(path, currentPathIndex, _dTimeDelta);
			_float3 direction = CurrentMobPos - GetTargetPos();
			GetTransform()->SetDirectionFixedUp(direction, _dTimeDelta, 5);
		}
	}
	else if(CurAnimState == UAnimationController::ANIM_ATTACK)
	{
		
		_float3 direction = CurrentMobPos - CurrentPlayerPos;
		GetTransform()->SetDirectionFixedUp(direction, _dTimeDelta, 5);
	}
	else if(CurAnimState == UAnimationController::ANIM_HIT)
	{
		
		GetTransform()->TranslateDir((GetTransform()->GetLook()), 1, 1);
	}
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	
	if (spGameInstance->GetDIKeyDown(DIK_1)) {
		m_spParticle->SetActive(true);
		m_spParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
	}
	else {
		//m_spParticle->SetActive(false);

	}

	if(CurAnimState == UAnimationController::ANIM_IDLE)
	{
		GetAnimModel()->TickAnimation(_dTimeDelta);
	}
	else if (CurAnimState == UAnimationController::ANIM_DEATH)
	{
		static _double elapsedTime = 0;
		_double DeathAnimSpeed = 20;
		elapsedTime += _dTimeDelta * DeathAnimSpeed;
		_double DeathTimeArcOpenEnd = 50;
		if (elapsedTime < DeathTimeArcOpenEnd)
			GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, elapsedTime);
	}
	else
		GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);

	_float newHeight = GetCurrentNavi()->ComputeHeight(GetTransform()->GetPos());
	GetTransform()->SetPos(_float3(GetTransform()->GetPos().x, newHeight, GetTransform()->GetPos().z));

	UpdateCollision();
}

void CMummy::LateTickActive(const _double& _dTimeDelta)
{
	for (auto& Colliders : GetColliderContainer())
	{
		Colliders.second->SetTransform(GetTransform()->GetWorldMatrix());
		Colliders.second->SetScale(_float3(3, 15, 3));
		Colliders.second->AddRenderer(RENDERID::RI_NONALPHA_LAST);
	}

	__super::LateTickActive(_dTimeDelta);
}

HRESULT CMummy::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CMummy::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void CMummy::Collision(CSHPTRREF<UPawn> _pEnemy)
{
	PAWNTYPE ePawnType = _pEnemy->GetPawnType();
	if (PAWNTYPE::PAWN_CHAR == ePawnType)
	{
		UCharacter* pCharacter = static_cast<UCharacter*>(_pEnemy.get());

		for(auto& iter : GetColliderContainer())
		{
			if (pCharacter->GetAnimModel()->IsCollisionAttackCollider(iter.second))
			{
				int a = 0;
			}
		}
	}
}


