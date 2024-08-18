#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CStatue.h"
#include "UPawn.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UModel.h"
#include "UProcessedData.h"
#include "CWarriorPlayer.h"
#include "CMainCamera.h"

CStatue::CStatue(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType)
{
}

CStatue::CStatue(const CStatue& _rhs)
	: CModelObjects(_rhs)
{
}

void CStatue::Free()
{
}

HRESULT CStatue::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CStatue::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SetModel(L"Proto_Res_Model_Statue_FBX.bin");

	STATUEDESC tBarsDesc = UMethod::ConvertTemplate_Index<STATUEDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tBarsDesc._Worldm);

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(15.f, 30.f, 15.f);

	SHPTR<UCollider> Collider2 = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring subColliderTag = L"ForInteractionStatue";
	AddColliderInContainer(subColliderTag, Collider2);

	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	/*SetOutline(true);*/
	SetIfOutlineScale(true);

	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTranslate(_float3(GetModel()->GetCenterPos().x, GetModel()->GetCenterPos().y - 10, GetModel()->GetCenterPos().z));
		Containers.second->SetTransform(GetTransform()->GetPos(), GetTransform()->GetQuaternion());
	}

	return S_OK;
}

void CStatue::TickActive(const _double& _dTimeDelta)
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

	if (false == GetInteractionState())
	{
		if (false == IsEnable())
		{
			spPlayer->SetCanInteractStatueState(false);
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
			spPlayer->SetCanInteractStatueState(true);

			if (GetCheckPointToOtherColor())
				spPlayer->SetDoneInteractStatueState(true);
			else
				spPlayer->SetDoneInteractStatueState(false);
		}
	}
}

void CStatue::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT CStatue::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	return S_OK;
}

HRESULT CStatue::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderShadowActive(_spCommand, _spTableDescriptor);
	return S_OK;
}
HRESULT CStatue::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	__super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
	return S_OK;
}

void CStatue::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

void CStatue::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
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
				spGameInstance->SoundPlayOnce(L"CheckpointSaving", GetTransform(), spPlayer->GetTransform());
				SetActiveEnable(true);
			}
			else if (2 == scStaticObjFind.enable())
			{
				spGameInstance->StopSound(L"CheckpointSaving");
				spGameInstance->SoundPlayOnce(L"CheckPointSave", GetTransform(), spPlayer->GetTransform());
				SetCheckPointToOtherColor(true);

				CS_SAVEPOINTENABLE csSavePointEnable;
				VECTOR3 vPos;
				_float3 vCurrentPos = GetTransform()->GetPos();
				PROTOFUNC::MakeVector3(&vPos, vCurrentPos.x, vCurrentPos.y, vCurrentPos.z);
				_int CellIndex = static_pointer_cast<CMainCamera>(spPlayer->GetFollowCamera())->GetCurrentNavi()->GetCurIndex();
				PROTOFUNC::MakeCsSavePointEnable(&csSavePointEnable, GetNetworkID(), vPos, CellIndex);
				spGameInstance->SendProtoData(UProcessedData(csSavePointEnable, TAG_CS_SAVEPOINTENABLE));
				SetEnable(false);
				SetOutline(false);
				spPlayer->SetCanInteractStatueState(false);
				spPlayer->SetDoneInteractStatueState(false);
				SetTickActive(false);
				spPlayer->SetInteractionElapsedTime(0);
			}
		}
		scStaticObjFind.Clear();
	}
	break;
	}
}

