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
	tDesc.vScale = _float3(1.f, 1.f, 1.f);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";
	AddColliderInContainer(mainColliderTag, Collider);

	SHPTR<UCollider> Collider2 = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring subColliderTag = L"ForInteraction";
	AddColliderInContainer(subColliderTag, Collider2);

	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	/*SetOutline(true);*/
	SetIfOutlineScale(true);
	return S_OK;
}

void CStatue::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	for (auto& Containers : GetColliderContainer())
	{
		if(Containers.first != L"ForInteraction")
		{
			Containers.second->SetTranslate(GetModel()->GetCenterPos());
			Containers.second->SetScaleToFitModel(GetModel()->GetMinVertexPos(), GetModel()->GetMaxVertexPos());
			Containers.second->SetTransform(GetTransform());
		}
		else
		{
			Containers.second->SetTranslate(GetModel()->GetCenterPos());
			Containers.second->SetScale(_float3(12, 20, 12));
			Containers.second->SetTransform(GetTransform());
		}
	}

	if (GetInteractionState())
	{
		
	}
}

void CStatue::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
	//for (auto& Containers : GetColliderContainer())
	//{
	//	if (Containers.first != L"ForInteraction")
	//	{
	//	}
	//	else
	//	{
	//		Containers.second->AddRenderer(RENDERID::RI_NONALPHA_LAST);
	//	}
	//}
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

