#include "ClientDefines.h"
#include "CMob.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "UMethod.h"
#include "CItemChest.h"
#include "CItemChestAnimController.h"
#include "UCollider.h"
#include "UPlayer.h"
#include "CWarriorPlayer.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UProcessedData.h"

CItemChest::CItemChest(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType), m_bisOpen{false}, m_ChestType{}
{
}

CItemChest::CItemChest(const CItemChest& _rhs)
	: CMob(_rhs), m_bisOpen{false}, m_ChestType{}
{
}

void CItemChest::Free()
{
}

HRESULT CItemChest::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CItemChest::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);

	SetActivationRange(10);
	SetDeactivationRange(20);

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(1.f, 1.f, 1.f);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";
	AddColliderInContainer(mainColliderTag, Collider);
	GetAnimModel()->SetAnimation(L"Open");

	SetOutlineColor(_float3(0, 1, 0));
	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTranslate(GetAnimModel()->GetCenterPos());
		Containers.second->SetScaleToFitModel(GetAnimModel()->GetMinVertexPos(), GetAnimModel()->GetMaxVertexPos());
	}
	{
		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEOPENCHEST2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLEOPENCHEST2DSHADER;


		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 1.3f;
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 0.05f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 1.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 2.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 2.1f;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 512;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.f, 0.f, 0.1f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticlePosition = _float3(0.f, 0.f, 0.f);
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleKind = PARTICLE_OPENCHEST;
		m_spOpenChestParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	m_spOpenChestParticle->GetParticleSystem()->GetParticleTypeParam()->fParticleType = PARTICLE_TYPE_DEFAULT;
	m_spOpenChestParticle->GetParticleSystem()->GetParticleTypeParam()->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
	m_spOpenChestParticle->SetParticleType(PARTICLE_OPENCHEST);
	*m_spOpenChestParticle->GetParticleSystem()->GetAddParticleAmount() = 2;
	*m_spOpenChestParticle->GetParticleSystem()->GetCreateInterval() = 1.1f;
	m_spOpenChestParticle->SetTexture(L"Twinkle");
	SetActive(true);
	GetAnimModel()->TickAnimation(0.0);

	for (auto& Containers : GetColliderContainer())
	{
		_float4x4 matrix = GetAnimModel()->GetPivotMatirx();
		matrix.Set_Pos(GetTransform()->GetPos());
		Containers.second->SetTransform(matrix);
	}
	m_ChestType = CHESTTYPE::TYPE_CHEST;
	return S_OK;
}

void CItemChest::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
{
	switch (_ProcessData.GetDataType())
	{
	case TAG_SC_STATICOBJFIND:
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		SHPTR<CWarriorPlayer> spPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->GetCurrPlayer());

		SC_STATICOBJFIND scStaticObjFind;
		scStaticObjFind.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());
		{
			SetOutline(true);
			SetIfOutlineScale(true);
			if (!m_bisOpen)
				spPlayer->SetCanInteractChestState(true);
			if (1 == scStaticObjFind.enable() && !m_bisOpen) {
				if (m_ChestType == TYPE_CHEST) {
					spPlayer->SetIfOpenChest(true);
					spGameInstance->SoundPlayOnce(L"ChestOpen", GetTransform(), spPlayer->GetTransform());
				}
				SetOpeningState(true);
			}
		}
		scStaticObjFind.Clear();
	}
	break;
	case TAG_SC_MONSTERSTATE:
	{
		static MOBSTATE MobState;
		MobState.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());

		if (true == MobState.triggeron())
		{
			GetAnimModel()->UpdateCurAnimationToRatio(MobState.animtime());
			SetElapsedTime(1000.0);
			m_spOpenChestParticle->SetActive(false);
		}
		MobState.Clear();
	}
	break;
	}
}

void CItemChest::TickActive(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	static _double elapsedTime = 0;
	_double ItemChestOpeningSpeed = 2;
	_double ItemChestTimeArcOpenEnd = 3;
	GetAnimationController()->Tick(_dTimeDelta);

	if (m_bisOpen)
	{
		if (m_ChestType == TYPE_CHEST)
		{
			if (!m_bisOpenSound) {
				spGameInstance->SoundPlayOnce(L"Heal", GetTransform(), GetTargetPlayer()->GetTransform());
				m_bisOpenSound = true;
			}

			ParticleActiveTime += _dTimeDelta;
			if (GetElapsedTime() < ItemChestTimeArcOpenEnd)
			{
				SetElapsedTime(GetElapsedTime() + _dTimeDelta * ItemChestOpeningSpeed);
				GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, GetElapsedTime());
			}
			m_spOpenChestParticle->SetActive(true);
			m_spOpenChestParticle->SetPosition(GetTransform()->GetPos());
			m_spOpenChestParticle->GetParticleSystem()->GetParticleParam()->stGlobalParticleInfo.fAccTime = 0.f;
		}
		else
		{
			spGameInstance->RemoveCollisionPawn(ThisShared<CMob>());
			SetActive(false);
		}
	}
	else
	{
		GetAnimModel()->TickAnimation(0.0);
	}
	if (ParticleActiveTime > 5.f) {
		m_spOpenChestParticle->SetActive(false);
	}
}

void CItemChest::LateTickActive(const _double& _dTimeDelta)
{
	if (true == GetOutlineState())
	{
		SetOutline(false);
		static_pointer_cast<CWarriorPlayer>(GetTargetPlayer())->SetCanInteractChestState(false);
	}

	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
	
	AddOutlineRenderGroup(RI_DEPTHRECORD);
	//AddNorPosRenderGroup(RI_NORPOS_FORABILITY);
	AddNorPosRenderGroup(RI_NORPOS);
}

HRESULT CItemChest::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CItemChest::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void CItemChest::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}
