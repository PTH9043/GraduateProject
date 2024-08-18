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
#include "UProcessedData.h"

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

void CMummy::CreateParticles()
{
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
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 212;
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
		m_spBloodParticle->SetBloodTexture(0, L"blood"); // y�� ���� x ��
		m_spBloodParticle->SetBloodTexture(1, L"blood1"); // y�� ���� x ��
		m_spBloodParticle->SetBloodTexture(2, L"blood2"); // y�� ���� x ��
		m_spBloodParticle->SetBloodTexture(3, L"blood3"); // y�� ���� x ��
		m_spBloodParticle->SetBloodTexture(4, L"blood4"); // y�� ���� x ��
		m_spBloodParticle->SetBloodTexture(5, L"blood5"); // y�� ���� x ��
		m_spBloodParticle->SetBloodTexture(6, L"blood6"); // y�� ���� x ��
		m_spBloodParticle->SetBloodTexture(7, L"blood7"); // y�� ���� x ��
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
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 0.30f;
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
		m_spSlashParticle->SetTexture(L"Slash3");

		m_spSlashParticle->SetParticleType(PARTICLE_SLASH);
		*m_spSlashParticle->GetParticleSystem()->GetCreateInterval() = 0.35f;
		*m_spSlashParticle->GetParticleSystem()->GetAddParticleAmount() = 1;
	}


	{

		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEATTACKEFFECT2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLEATTACK2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 0.4f;///0.4;//	 0.8f
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 0.2f;///0.2;//	 1.0f
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 1.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 1.88f;//3.25f
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 1.88f;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 512;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.f, 0.f, 0.f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticlePosition = _float3(0.f, 0.f, 0.f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleKind = PARTICLE_ATTACK;
		m_spAttackParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	{
		m_spAttackParticle->GetParticleSystem()->GetParticleTypeParam()->fParticleType = PARTICLE_TYPE_DEFAULT;
		m_spAttackParticle->GetParticleSystem()->GetParticleTypeParam()->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
		m_spAttackParticle->SetParticleType(PARTICLE_ATTACK);
		*m_spAttackParticle->GetParticleSystem()->GetAddParticleAmount() = 4;
		*m_spAttackParticle->GetParticleSystem()->GetCreateInterval() = 0.5f;
		m_spAttackParticle->SetTexture(L"FireSpark3");
	}
	{

		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEATTACKEFFECT2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLEATTACKTWO2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 1.0f;///0.4;//	 0.8f
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 1.2f;///0.2;//	 1.0f
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 1.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 7.25f;//3.25f
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 1.88f;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 212;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.f, 0.f, 0.f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticlePosition = _float3(0.f, 0.f, 0.f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleKind = PARTICLE_ATTACK;
		m_spAttackParticleTwo = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	{
		m_spAttackParticleTwo->GetParticleSystem()->GetParticleTypeParam()->fParticleType = PARTICLE_TYPE_DEFAULT;
		m_spAttackParticleTwo->GetParticleSystem()->GetParticleTypeParam()->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
		m_spAttackParticleTwo->SetParticleType(PARTICLE_ATTACK);
		*m_spAttackParticleTwo->GetParticleSystem()->GetAddParticleAmount() = 3;
		*m_spAttackParticleTwo->GetParticleSystem()->GetCreateInterval() = 0.5f;
		m_spAttackParticleTwo->SetTexture(L"FireSpark3");
	}
	{
		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEFOOTPRINT2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLEFOOTPRINT2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 7.5f;// 6.5f
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 4.5f; //3.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 2.0f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 50;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = -GetTransform()->GetLook();
		tDesc.ParticleParam.stGlobalParticleInfo.fParticlePosition = GetTransform()->GetPos();
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleKind = PARTICLE_FOOTPRINT;
		m_spFootPrintParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));

		m_stParticleParam = m_spFootPrintParticle->GetParticleSystem()->GetParticleParam();
		m_stParticleType = m_spFootPrintParticle->GetParticleSystem()->GetParticleTypeParam();
		m_stParticleType->fParticleType = PARTICLE_TYPE_AUTO;
		m_stParticleType->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
		m_spFootPrintParticle->SetTexture(L"Sand");// DUST 


		*m_spFootPrintParticle->GetParticleSystem()->GetCreateInterval() = 0.8f;
		*m_spFootPrintParticle->GetParticleSystem()->GetAddParticleAmount() = 1;
		m_spFootPrintParticle->SetParticleType(PARTICLE_FOOTPRINT);
		m_spFootPrintParticle->SetActive(false);
	}
}

HRESULT CMummy::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	CreateParticles();

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

	SetHealth(500);
	SetMaxHealth(500);
	SetActivationRange(50);
	SetDeactivationRange(80);
	SetOutlineByAbility(true);
	SetOutline(true);
	SetOutlineColor(_float3(1, 0, 0));
	SetActive(true);
	SetMovingSpeed(5.f);

	return S_OK;
}

void CMummy::TickActive(const _double& _dTimeDelta)
{
	GetAnimationController()->Tick(_dTimeDelta);
	__super::TickActive(_dTimeDelta);
	_float3 pos = GetTransform()->GetPos();
	pos.y += 5;

	m_spBloodParticle->SetPosition(pos);
	m_spSlashParticle->SetPosition(pos);
	m_spAttackParticle->SetPosition(pos);
	m_spAttackParticleTwo->SetPosition(pos);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<UPlayer> spPlayer = GetTargetPlayer();
	// death animation
	if (GetDeathState())
	{
		_double DeathAnimSpeed = 20;
		if (GetElapsedTime() == 0.0)
		{
			USound* DeathSound1 = spGameInstance->BringSound(L"Death_VO_1").get();
			USound* DeathSound2 = spGameInstance->BringSound(L"BodyHitFloor_1").get();
			DeathSound1->PlayWithInputChannel(&m_pDeathChannel, GetTransform(), spPlayer->GetTransform());
			DeathSound2->PlayWithInputChannel(&m_pDeath2Channel, GetTransform(), spPlayer->GetTransform());
		}
		SetElapsedTime(GetElapsedTime() + (_dTimeDelta * DeathAnimSpeed));
		_double DeathTimeArcOpenEnd = 500;
		if (GetElapsedTime() < DeathTimeArcOpenEnd)
		{
			GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, GetElapsedTime());
			GetAnimModel()->UpdateDissolveTImer(_dTimeDelta);
			if (!m_bDissolveSound) {
				SHPTR<USound> DsSound = spGameInstance->BringSound(L"DissolveSound");
				DsSound->PlayWithInputChannel(&m_pDissolveChannel, GetTransform(), spPlayer->GetTransform());
				m_bDissolveSound = true;
			}
		}
		else
		{
			SetDeadDissolveEnable(true);
		}
	}
	else
	{
		_int CurAnimState = GetAnimationController()->GetAnimState();
		{
			if (CurAnimState == UAnimationController::ANIM_MOVE)
			{
				AddTimeAccumulator(_dTimeDelta);
				// A* for moving towards player when player is found
				if (GetFoundTargetState())
				{
					m_spFootPrintParticle->SetActive(true);
					{
						*m_spFootPrintParticle->GetParticleSystem()->GetAddParticleAmount() = 4;
						*m_spFootPrintParticle->GetParticleSystem()->GetCreateInterval() = 0.355f;
						_float3 pos = GetTransform()->GetPos() + GetTransform()->GetRight();
						pos.y += 0.5;
						_float3 Look = GetTransform()->GetLook();
						_float3 Right = 1.2f * GetTransform()->GetRight();
						//pos -= 3 * Look;
						m_spFootPrintParticle->SetPosition(pos);
						m_spFootPrintParticle->SetDirection(Right);
					}
					SetOutline(true);
				}
				else // patrolling when player is not found
				{
//					SetOutline(false);
				}
			}
			else if (CurAnimState == UAnimationController::ANIM_ATTACK) {}
			else {
				m_spFootPrintParticle->SetActive(false);
				*m_spFootPrintParticle->GetParticleSystem()->GetAddParticleAmount() = 0;
				*m_spFootPrintParticle->GetParticleSystem()->GetCreateInterval() = 0.8f;
			}
		}
	}

	GetAnimModel()->TickAnimation(_dTimeDelta);
	if (spGameInstance->GetDIKeyPressing(DIK_H)) {
		SetIfOutlineScale(true);
	}
	else {
		SetIfOutlineScale(false);
	}

	UpdateCollision();
}

void CMummy::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	_int CurAnimState = GetAnimationController()->GetAnimState();
	if (CurAnimState == UAnimationController::ANIM_DEATH)
	{
		if (GetElapsedTime() >= 100.0)
		{
			m_spBloodParticle->SetActive(false);
			m_spSlashParticle->SetActive(false);
			m_spFootPrintParticle->SetActive(false);
			m_spAttackParticle->SetActive(false);
			m_spAttackParticleTwo->SetActive(false);
			SetOutline(false);
			SetActive(false);
			spGameInstance->RemoveCollisionPawn(ThisShared<CMob>());
		}
	}
}

HRESULT CMummy::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	const _wstring& CurAnimName = GetAnimModel()->GetCurrentAnimation()->GetAnimName();
	
	if(CurAnimName != L"staticLaying" && CurAnimName != L"staticStanding")
	{
		__super::RenderActive(_spCommand, _spTableDescriptor);
	}
	
	return S_OK;
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
	__super::Collision(_pEnemy, _dTimeDelta);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	PAWNTYPE ePawnType = _pEnemy->GetPawnType();
	const _wstring& CurAnimName = GetAnimModel()->GetCurrentAnimation()->GetAnimName();

	_float3 direction = (GetTransform()->GetPos() - _pEnemy->GetTransform()->GetPos());
	direction.Normalize();
	_bool IsHit = true;

	auto handleCollisionWithPlayer = [&](UCharacter* pCharacter) {
		for (const auto& iter : GetColliderContainer())
		{
			if (pCharacter->GetAnimModel()->IsCollisionAttackCollider(iter.second))
			{
				if (CurAnimName != L"openLaying" && CurAnimName != L"openStanding" &&
					CurAnimName != L"death")
				{
					if (!GetIsHItAlreadyState())
					{
						// Decrease health on hit
						SendCollisionData(_pEnemy.get(), 100);
					}
					
					SetHitAlreadyState(true);
				}
			}
			else
			{
				SetHitAlreadyState(false);
				SetAnimModelRim(false);
			}
		}
		};

	if (ePawnType == PAWNTYPE::PAWN_PLAYER)
	{
		UCharacter* pCharacter = static_cast<UCharacter*>(_pEnemy.get());
		handleCollisionWithPlayer(pCharacter);
	}
}

void CMummy::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
{
	switch (_ProcessData.GetDataType())
	{
	case TAG_SC_MONSTERSTATE:
	{
		static MOBSTATE MobState;
		MobState.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());

		if (false == MobState.triggeron())
		{
			GetAnimationController()->ReceiveNetworkProcessData(&MobState);
			GetTransform()->SetPos(_float3{ MobState.posx(), MobState.posy(), MobState.posz() });
			GetTransform()->RotateFix(_float3{ MobState.rotatex(), MobState.rotatey(), MobState.rotatez() });
		}
		else
		{
			GetAnimModel()->UpdateCurAnimationToRatio(MobState.animtime());
			SetDeathState(true);
			SetActive(false);
			SetDeadDissolveEnable(true);
			SetElapsedTime(1000.0);
		}
		MobState.Clear();
	}
	break;
	case TAG_SC_DAMAGED:
	{
		static SC_DAMAGED scDamaged;
		scDamaged.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());
		if (0 >= scDamaged.hp())
		{
			SetDeathState(true);
			SetHealth(0);
			SetOutline(false);
		}
		else
		{
			SetHealth(scDamaged.hp());
			SetDamaged(true);
		}
		m_spBloodParticle->SetActive(true);
		m_spAttackParticle->SetActive(true);
		m_spAttackParticleTwo->SetActive(true);
		m_spSlashParticle->SetActive(true);
		SetAnimModelRim(true);
		m_spBloodParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		m_spAttackParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		m_spAttackParticleTwo->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		m_spSlashParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		scDamaged.Clear();
	}
	break;
	}
}


