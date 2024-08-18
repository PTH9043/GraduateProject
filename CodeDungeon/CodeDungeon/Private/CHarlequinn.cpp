#include "ClientDefines.h"
#include "CHarlequinn.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "URegion.h"
#include "UNavigation.h"
#include "CHarlequinnAnimController.h"
#include "UCell.h"
#include "UMethod.h"
#include "UPlayer.h"
#include "UCollider.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "CModelObjects.h"
#include "UAnimation.h"
#include "CShuriken.h"
#include "CShurikenThrowing.h"
#include "UProcessedData.h"
#include "CShuriken.h"

CHarlequinn::CHarlequinn(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType),
	m_PathFindingState{},
	m_AstarPath{},
	m_isPathFinding{ false },
	m_currentPathIndex{ 0 },
	m_spShurikens{nullptr},
	m_spShurikensForThrowing{nullptr}
{
}

CHarlequinn::CHarlequinn(const CHarlequinn& _rhs)
	: CMob(_rhs),
	m_PathFindingState{},
	m_AstarPath{},
	m_isPathFinding{ false },
	m_currentPathIndex{ 0 },
	m_spShurikens{ nullptr },
	m_spShurikensForThrowing{ nullptr }
{
}

void CHarlequinn::Free()
{
}

HRESULT CHarlequinn::NativeConstruct()
{
	return __super::NativeConstruct();
}

void CHarlequinn::CreateParticles()
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

		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEATTACKEFFECT2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLEATTACK2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 0.45f;  //0.45f;  //0.85 scale 끌때
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 0.05f;  //0.05f;  //1.0 
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 1.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 2.88f;
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
		*m_spAttackParticle->GetParticleSystem()->GetAddParticleAmount() = 7;
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
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 512;
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
		*m_spFootPrintParticle->GetParticleSystem()->GetAddParticleAmount() = 2;
		m_spFootPrintParticle->SetParticleType(PARTICLE_FOOTPRINT);
		m_spFootPrintParticle->SetActive(false);
	}
}

HRESULT CHarlequinn::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	CreateParticles();

	//장식 표창
	{
		CShuriken::EQDESC Desc1(std::static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_SHURIKENMODEL)), ThisShared<UCharacter>(), L"..\\..\\Resource\\Model\\Item\\Equip\\Shuriken\\Convert\\EquipDesc\\Shuriken_FBX.bin");
		CShuriken::EQDESC Desc2(std::static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_SHURIKENMODEL)), ThisShared<UCharacter>(), L"..\\..\\Resource\\Model\\Item\\Equip\\Shuriken\\Convert\\EquipDesc\\Shuriken_FBX1.bin");
		CShuriken::EQDESC Desc3(std::static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_SHURIKENMODEL)), ThisShared<UCharacter>(), L"..\\..\\Resource\\Model\\Item\\Equip\\Shuriken\\Convert\\EquipDesc\\Shuriken_FBX2.bin");
		CShuriken::EQDESC Desc4(std::static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_SHURIKENMODEL)), ThisShared<UCharacter>(), L"..\\..\\Resource\\Model\\Item\\Equip\\Shuriken\\Convert\\EquipDesc\\Shuriken_FBX3.bin");

		SHPTR<CShuriken> Shuriken = std::static_pointer_cast<CShuriken>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SHURIKEN, { &Desc1 }));
		m_spShurikens.push_back(Shuriken);
		Shuriken = std::static_pointer_cast<CShuriken>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SHURIKEN, { &Desc2 }));
		m_spShurikens.push_back(Shuriken);
		Shuriken = std::static_pointer_cast<CShuriken>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SHURIKEN, { &Desc3 }));
		m_spShurikens.push_back(Shuriken);
		Shuriken = std::static_pointer_cast<CShuriken>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SHURIKEN, { &Desc4 }));
		m_spShurikens.push_back(Shuriken);
	}

	m_spShurikensForThrowing = Create<VECTOR<SHPTR<CShurikenThrowing>>>();

	CShurikenThrowing::SHURIKENDESC	Desc{};
	Desc._Worldm = GetTransform()->GetWorldMatrix();
	SHPTR<CShurikenThrowing> ShurikenThrowing = std::static_pointer_cast<CShurikenThrowing>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SHURIKENTHROWING, { &Desc }));
	m_spShurikensForThrowing->push_back(ShurikenThrowing);
	spGameInstance->AddCollisionPawnList(ShurikenThrowing);
	for(int i = 0; i < 5; i++)
	{
		ShurikenThrowing = std::static_pointer_cast<CShurikenThrowing>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SHURIKENTHROWING, { &Desc }));
		m_spShurikensForThrowing->push_back(ShurikenThrowing);
		spGameInstance->AddCollisionPawnList(ShurikenThrowing);
	}
	

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(0, 0, 0);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";

	AddColliderInContainer(mainColliderTag, Collider);
	for (auto& Colliders : GetColliderContainer())
	{
		Colliders.second->SetScale(_float3(3, 10, 3));
		Colliders.second->SetTranslate(_float3(0, 10, 0));
	}

	SetHealth(1500);
	SetMaxHealth(1500);

	SetActivationRange(80);
	SetDeactivationRange(150);
	SetOutlineByAbility(true);
	SetOutline(true);
	SetOutlineColor(_float3(1, 1, 0));
	SetMobType(TAG_HARLEQUINN);
	return S_OK;
}

void CHarlequinn::ThrowShurikens(_int _shurikenIndex, const _double& _dTimeDelta, const _float3& _dir)
{
	(*m_spShurikensForThrowing)[_shurikenIndex]->ThrowShurikens(_dTimeDelta, _dir);
}

void CHarlequinn::TickActive(const _double& _dTimeDelta)
{
	GetAnimationController()->Tick(_dTimeDelta);
	__super::TickActive(_dTimeDelta);
	_float3 pos = GetTransform()->GetPos();
	pos.y += 5;
	SHPTR<UPlayer> spPlayer = GetTargetPlayer();
	m_spBloodParticle->SetPosition(pos);
	m_spSlashParticle->SetPosition(pos);
	m_spAttackParticle->SetPosition(pos);
	m_spAttackParticleTwo->SetPosition(pos);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (GetDeathState())
	{
		_double DeathAnimSpeed = 20;
		SetElapsedTime(GetElapsedTime() + (_dTimeDelta * DeathAnimSpeed));
		_double DeathTimeArcOpenEnd = 50;
		if (GetElapsedTime() < DeathTimeArcOpenEnd) {
			GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, GetElapsedTime());
			GetAnimModel()->UpdateDissolveTImer(_dTimeDelta * 1.2f);
			if (!m_bDissolveSound) {
				spGameInstance->SoundPlayOnce(L"DissolveSound2", GetTransform(), spPlayer->GetTransform());
				m_bDissolveSound = true;
			}
			for (auto& iter : (*m_spShurikensForThrowing))
				iter->SetActive(false);

			for (auto& iter : m_spShurikens)
			{
				if (nullptr != iter)
					iter->SetActive(false);
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
//				SetOutline(false);
			}
		}
		else if (CurAnimState == UAnimationController::ANIM_ATTACK) {}
		else {
			m_spFootPrintParticle->SetActive(false);
			*m_spFootPrintParticle->GetParticleSystem()->GetAddParticleAmount() = 0;
			*m_spFootPrintParticle->GetParticleSystem()->GetCreateInterval() = 0.8f;
		}
		SetElapsedTime(0.f);
	}

	GetAnimModel()->TickAnimation(_dTimeDelta);
	UpdateCollision();
}

void CHarlequinn::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	_int CurAnimState = GetAnimationController()->GetAnimState();
	if (CurAnimState == UAnimationController::ANIM_DEATH)
	{
		if (GetElapsedTime() >= 100.0)
		{
			m_spBloodParticle->SetActive(false);
			m_spFootPrintParticle->SetActive(false);
				m_spSlashParticle->SetActive(false);
				m_spAttackParticle->SetActive(false);
				m_spAttackParticleTwo->SetActive(false);
			SetActive(false);
			SetOutline(false);
			spGameInstance->RemoveCollisionPawn(ThisShared<CMob>());
		}
	}
}

HRESULT CHarlequinn::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CHarlequinn::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderShadowActive(_spCommand, _spTableDescriptor);
}

HRESULT CHarlequinn::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	return __super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
}

void CHarlequinn::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
	__super::Collision(_pEnemy, _dTimeDelta);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	PAWNTYPE ePawnType = _pEnemy->GetPawnType();
	const _wstring& CurAnimName = GetAnimModel()->GetCurrentAnimation()->GetAnimName();

	_float3 direction = (_pEnemy->GetTransform()->GetPos() - GetTransform()->GetPos());
	direction.Normalize();

	_float3 forward = GetTransform()->GetLook();
	_float dotProduct = forward.Dot(direction);

	auto handleCollisionWithPlayer = [&](UCharacter* pCharacter) {
		for (const auto& iter : GetColliderContainer())
		{
			if (pCharacter->GetAnimModel()->IsCollisionAttackCollider(iter.second))
			{
				if (!GetIsHItAlreadyState())
				{
					//등을 맞았을 때에만
					if (dotProduct < -0.5f)
					{
						SendCollisionData(_pEnemy.get(), 200);
					}
					else
					{
						// Decrease health on hit
							// Decrease health on hit
						SendCollisionData(_pEnemy.get(), 50);
					}
				}

				SetHitAlreadyState(true);
			}
			else
			{
				SetHitAlreadyState(false);
			}


			for (const auto& iter2 : pCharacter->GetColliderContainer())
			{
				if (iter2.first == L"Main")
				{
					if (iter.second->IsCollision(iter2.second))
					{
						SetCollisionState(true);
						GetTransform()->SetPos(GetTransform()->GetPos() - direction * 10 * _dTimeDelta);
					}
					else
					{
						SetCollisionState(false);
					}
				}
			}
		}
		};

	if (ePawnType == PAWNTYPE::PAWN_PLAYER)
	{
		UCharacter* pCharacter = static_cast<UCharacter*>(_pEnemy.get());
		handleCollisionWithPlayer(pCharacter);
	}
}

void CHarlequinn::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
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
		}
		else
		{
			SetHealth(scDamaged.hp());
			SetDamaged(true);
		}
		m_spBloodParticle->SetActive(true);
		m_spSlashParticle->SetActive(true);
		m_spAttackParticle->SetActive(true);
		m_spAttackParticleTwo->SetActive(true);
		m_spBloodParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		m_spSlashParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		m_spAttackParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		m_spAttackParticleTwo->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		scDamaged.Clear();
	}
	break;
	}
}
