#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CIronBars.h"
#include "UPawn.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UModel.h"
#include "UProcessedData.h"
#include "CWarriorPlayer.h"

CIronBars::CIronBars(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType),
	m_SoundTimer{4}, m_isEnable{false}
{
}

CIronBars::CIronBars(const CIronBars& _rhs)
	: CModelObjects(_rhs)
{
}

void CIronBars::Free()
{
}

HRESULT CIronBars::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CIronBars::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SetModel(L"Proto_Res_Model_Bars_FBX.bin");

	IRONBARSDESC tBarsDesc = UMethod::ConvertTemplate_Index<IRONBARSDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tBarsDesc._Worldm);
	m_f3OriginPos = GetTransform()->GetPos();

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(410, 350, 0);
	SHPTR<UCollider> Collider1 = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";
	AddColliderInContainer(mainColliderTag, Collider1);


	UCollider::COLLIDERDESC tDesc2;
	tDesc2.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc2.vScale = _float3(410, 350, 100);
	SHPTR<UCollider> Collider2 = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc2 }));
	_wstring subColliderTag = L"ForInteractionBars";
	AddColliderInContainer(subColliderTag, Collider2);


	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	SetIfOutlineScale(false);

	/*SetOutline(true);*/
	return S_OK;
}

void CIronBars::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	if (false == m_isEnable)
	{
		if (m_SoundTimer.fTimer > 0)
		{
			spGameInstance->StopSound(L"BarLift");
			m_SoundTimer.ResetTimer();
		}
	}

	if (GetInteractionState())
	{
		if (GetTransform()->GetPos().y - m_f3OriginPos.y < 30)
			GetTransform()->TranslateDir(_float3(0, 1, 0), _dTimeDelta, 3);
		else
			SetActive(false);

		for (auto& Containers : GetColliderContainer())
		{
			Containers.second->SetTranslate(GetModel()->GetCenterPos());
			Containers.second->SetTransform(GetTransform());
		}
	}
}


void CIronBars::LateTickActive(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<CWarriorPlayer> spPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->GetCurrPlayer());

	if (false == m_isEnable)
	{
		spPlayer->SetCanInteractBarState(false);
		SetOutline(false);
		spGameInstance->StopSound(L"BarLift");
	}
	else
	{
		SetOutline(true);
		spPlayer->SetCanInteractBarState(true);

		if (GetCheckPointToOtherColor())
			spPlayer->SetDoneInteractBarState(true);
		else
			spPlayer->SetDoneInteractBarState(false);
	}
	m_isEnable = false;

	__super::LateTickActive(_dTimeDelta);
}

void CIronBars::NetworkTickActive(const _double& _dTimeDelta)
{
}

HRESULT CIronBars::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	return S_OK;
}

HRESULT CIronBars::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderShadowActive(_spCommand, _spTableDescriptor);
	return S_OK;
}
HRESULT CIronBars::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	__super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
	return S_OK;
}

void CIronBars::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

void CIronBars::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
{
	m_isEnable = true;
	switch (_ProcessData.GetDataType())
	{
	case TAG_SC_STATICOBJFIND:
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		SHPTR<CWarriorPlayer> spPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->GetCurrPlayer());

		SC_STATICOBJFIND scStaticObjFind;
		scStaticObjFind.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());
		{
			if (0 == scStaticObjFind.enable())
			{

			}
			else if(1 == scStaticObjFind.enable())
			{
				spGameInstance->SoundPlayOnce(L"BarLift");
			}
			else
			{
				spGameInstance->StopSound(L"BarLift");
				spGameInstance->SoundPlayOnce(L"BarLiftStart");
				spGameInstance->SoundPlayOnce(L"BarLift2");
				SetInteractionState(true);
				SetCheckPointToOtherColor(true);
				m_SoundTimer.ResetTimer();
			}
		}
		scStaticObjFind.Clear();
	}
		break;
	}
}

