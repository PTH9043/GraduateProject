#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CCoreAnubis.h"
#include "UPawn.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UModel.h"
#include "UProcessedData.h"
#include "CWarriorPlayer.h"

CCoreAnubis::CCoreAnubis(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType)
{
}

CCoreAnubis::CCoreAnubis(const CCoreAnubis& _rhs)
	: CModelObjects(_rhs)
{
}

void CCoreAnubis::Free()
{
}

HRESULT CCoreAnubis::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CCoreAnubis::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SetModel(L"Proto_Res_Model_anubishead_FBX.bin");

	COREANUBISNDESC tBarsDesc = UMethod::ConvertTemplate_Index<COREANUBISNDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tBarsDesc._Worldm);
	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	SetActive(false);

	SetIfOutlineScale(true);

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(-10.f, 10.f, 15.f);
	tDesc.vScale = _float3(12.5, 12.5, 12.5);
	SHPTR<UCollider> Collider1 = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_SPHERECOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"ForInteractionCoreAnubis";
	AddColliderInContainer(mainColliderTag, Collider1);

	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTransform(GetTransform()->GetPos(), GetTransform()->GetQuaternion());
	}
	return S_OK;
}

void CCoreAnubis::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<CWarriorPlayer> spPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->GetCurrPlayer());
	if (_float3::Distance(spPlayer->GetTransform()->GetPos(), GetTransform()->GetPos()) >= 10.f)
	{
		SetEnable(false);
	}
	else
	{
		SetEnable(true);
	}

	if (false == IsEnable())
	{
		spPlayer->SetCanInteractAnubisCoreState(false);
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
		spPlayer->SetCanInteractAnubisCoreState(true);

		if (GetCheckPointToOtherColor())
			spPlayer->SetDoneInteractAnubisCoreState(true);
		else
			spPlayer->SetDoneInteractAnubisCoreState(false);
	}
	SetEnable(false);
}

void CCoreAnubis::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT CCoreAnubis::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	return S_OK;
}

HRESULT CCoreAnubis::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderShadowActive(_spCommand, _spTableDescriptor);
	return S_OK;
}
HRESULT CCoreAnubis::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	__super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
	return S_OK;
}

void CCoreAnubis::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

void CCoreAnubis::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
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
				spGameInstance->SoundPlayOnce(L"AnubisCore", GetTransform(), spPlayer->GetTransform());
				SetActiveEnable(true);
			}
			else if (2 == scStaticObjFind.enable())
			{
				spGameInstance->StopSound(L"AnubisCore");
				spGameInstance->SoundPlayOnce(L"CoreComplete3", GetTransform(), spPlayer->GetTransform());
				SetCheckPointToOtherColor(true);

				CS_CORENABLE csCoreEnable;
				VECTOR3 vPos;
				_float3 vCurrentPos = GetTransform()->GetPos();
				PROTOFUNC::MakeCsCoreEnable(&csCoreEnable, GetNetworkID());
				spGameInstance->SendProtoData(UProcessedData(csCoreEnable, TAG_CS_COREENABLE));

				SetEnable(false);
				SetOutline(false);
				spPlayer->SetCanInteractAnubisCoreState(false);
				spPlayer->SetDoneInteractAnubisCoreState(false);
				SetTickActive(false);
				spPlayer->SetInteractionElapsedTime(0);
			}
		}
		scStaticObjFind.Clear();
	}
	break;
	}
}

