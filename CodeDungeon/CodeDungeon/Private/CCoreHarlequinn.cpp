#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CCoreHarlequinn.h"
#include "UPawn.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UModel.h"
#include "UProcessedData.h"
#include "CWarriorPlayer.h"

CCoreHarlequinn::CCoreHarlequinn(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType)
{
}

CCoreHarlequinn::CCoreHarlequinn(const CCoreHarlequinn& _rhs)
	: CModelObjects(_rhs)
{
}

void CCoreHarlequinn::Free()
{
}

HRESULT CCoreHarlequinn::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CCoreHarlequinn::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SetModel(L"Proto_Res_Model_HarlequinnHead_FBX.bin");

	COREHARLEQUINNDESC tBarsDesc = UMethod::ConvertTemplate_Index<COREHARLEQUINNDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tBarsDesc._Worldm);
	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	SetActive(false);
	

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 37.f, 0.f);
	tDesc.vScale = _float3(12.5, 12.5, 12.5);
	SHPTR<UCollider> Collider1 = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_SPHERECOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"ForInteractionCoreHarlequinn";
	AddColliderInContainer(mainColliderTag, Collider1);

	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTransform(GetTransform()->GetPos(), GetTransform()->GetQuaternion());
	}

	return S_OK;
}

void CCoreHarlequinn::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<CWarriorPlayer> spPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->GetCurrPlayer());
	if (_float3::Distance(spPlayer->GetTransform()->GetPos(), GetTransform()->GetPos()) >= 30.f)
	{
		SetEnable(false);
	}
	else
	{
		SetEnable(true);
	}

	if (false == IsEnable())
	{
		spPlayer->SetCanInteractHarlCoreState(false);
		SetOutline(false);
		if (true == IsActiveEnable())
		{
			spPlayer->SetInteractionElapsedTime(0);
			SetActiveEnable(false);
		}
	}
	else
	{
		if (true == IsActiveEnable())
		{
			spPlayer->SetInteractionElapsedTime(spPlayer->GetInteractionElapsedTime() + (_float)(_dTimeDelta));
			if (false == spGameInstance->GetDIKeyPressing(DIK_F))
			{
				spPlayer->SetInteractionElapsedTime(0);
				SetActiveEnable(false);
			}
		}
		SetOutline(true);
		spPlayer->SetCanInteractHarlCoreState(true);

		if (GetCheckPointToOtherColor())
			spPlayer->SetDoneInteractHarlequinnCoreState(true);
		else
			spPlayer->SetDoneInteractHarlequinnCoreState(false);
	}
	SetEnable(false);
}


void CCoreHarlequinn::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}
HRESULT CCoreHarlequinn::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	return S_OK;
}

HRESULT CCoreHarlequinn::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderShadowActive(_spCommand, _spTableDescriptor);
	return S_OK;
}
HRESULT CCoreHarlequinn::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	__super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
	return S_OK;
}

void CCoreHarlequinn::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

void CCoreHarlequinn::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
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
			if (1 == scStaticObjFind.enable())
			{
				spGameInstance->SoundPlayOnce(L"HalequinCore", GetTransform(), spPlayer->GetTransform());
				SetActiveEnable(true);
			}
			else if (2 == scStaticObjFind.enable())
			{
				spGameInstance->StopSound(L"HalequinCore");
				spGameInstance->SoundPlayOnce(L"CoreComplete2", GetTransform(), spPlayer->GetTransform());
				SetCheckPointToOtherColor(true);

				CS_CORENABLE csCoreEnable;
				VECTOR3 vPos;
				_float3 vCurrentPos = GetTransform()->GetPos();
				PROTOFUNC::MakeCsCoreEnable(&csCoreEnable, GetNetworkID());
				spGameInstance->SendProtoData(csCoreEnable, TAG_CS_COREENABLE);

				SetEnable(false);
				SetOutline(false);
				spPlayer->SetCanInteractHarlCoreState(false);
				SetTickActive(false);
				spPlayer->SetDoneInteractHarlequinnCoreState(true);
			}
		}
		scStaticObjFind.Clear();
	}
	break;
	}
}

