#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CCoreMinotaur.h"
#include "UPawn.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UModel.h"
#include "UProcessedData.h"
#include "CWarriorPlayer.h"

CCoreMinotaur::CCoreMinotaur(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType)
{
}

CCoreMinotaur::CCoreMinotaur(const CCoreMinotaur& _rhs)
	: CModelObjects(_rhs)
{
}

void CCoreMinotaur::Free()
{
}

HRESULT CCoreMinotaur::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CCoreMinotaur::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SetModel(L"Proto_Res_Model_minotaurhead_FBX.bin");

	COREMINOTAURDESC tBarsDesc = UMethod::ConvertTemplate_Index<COREMINOTAURDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tBarsDesc._Worldm);
	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	SetActive(false);
	
	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 37.f, -5.f);
	tDesc.vScale = _float3(10, 10, 10);
	SHPTR<UCollider> Collider1 = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_SPHERECOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"ForInteractionCoreMinotaur";
	AddColliderInContainer(mainColliderTag, Collider1);

	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTransform(GetTransform()->GetPos(), GetTransform()->GetQuaternion());
	}
	return S_OK;
}

void CCoreMinotaur::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<CWarriorPlayer> spPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->GetCurrPlayer());
	if (_float3::Distance(spPlayer->GetTransform()->GetPos(), GetTransform()->GetPos()) >= 12.f)
	{
		SetEnable(false);
	}
	else
	{
		SetEnable(true);
	}

	if (false == IsEnable())
	{
		spPlayer->SetCanInteractMinoCoreState(false);
		SetOutline(false);
		if (true == IsActiveEnable())
		{
			spPlayer->SetInteractionElapsedTime(0);
			SetActiveEnable(false);
		}
	}
	else
	{
		SetOutline(true);
		spPlayer->SetCanInteractMinoCoreState(true);
		if (true == IsActiveEnable())
		{
			spPlayer->SetInteractionElapsedTime(spPlayer->GetInteractionElapsedTime() + (_float)(_dTimeDelta));
			if (false == spGameInstance->GetDIKeyPressing(DIK_F))
			{
				spPlayer->SetInteractionElapsedTime(0);
				SetActiveEnable(false);
			}
		}

		if (GetCheckPointToOtherColor())
			spPlayer->SetDoneInteractMinoCoreState(true);
		else
			spPlayer->SetDoneInteractMinoCoreState(false);
	}
}


void CCoreMinotaur::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);

}

HRESULT CCoreMinotaur::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	return S_OK;
}

HRESULT CCoreMinotaur::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderShadowActive(_spCommand, _spTableDescriptor);
	return S_OK;
}
HRESULT CCoreMinotaur::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	__super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
	return S_OK;
}

void CCoreMinotaur::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

void CCoreMinotaur::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
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
				spGameInstance->SoundPlayOnce(L"MinoCore", GetTransform(), spPlayer->GetTransform());
				SetActiveEnable(true);
			}
			else if (2 == scStaticObjFind.enable())
			{
				spGameInstance->StopSound(L"MinoCore");
				spGameInstance->SoundPlayOnce(L"CoreComplete", GetTransform(), spPlayer->GetTransform());
				SetCheckPointToOtherColor(true);

				CS_CORENABLE csCoreEnable;
				VECTOR3 vPos;
				_float3 vCurrentPos = GetTransform()->GetPos();
				PROTOFUNC::MakeCsCoreEnable(&csCoreEnable, GetNetworkID());
				spGameInstance->SendProtoData(UProcessedData(csCoreEnable, TAG_CS_COREENABLE));

				SetEnable(false);
				SetOutline(false);
				spPlayer->SetCanInteractMinoCoreState(false);
				spPlayer->SetDoneInteractMinoCoreState(false);
				SetTickActive(false);
				spPlayer->SetInteractionElapsedTime(0);
			}
		}
		scStaticObjFind.Clear();
	}
	break;
	}
}

