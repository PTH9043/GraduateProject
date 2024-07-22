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

	SetModel(L"Proto_Res_Model_Bars_FBX.bin");

	COREMINOTAURDESC tBarsDesc = UMethod::ConvertTemplate_Index<COREMINOTAURDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tBarsDesc._Worldm);

	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(1.f, 1.f, 1.f);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";
	AddColliderInContainer(mainColliderTag, Collider);

	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);

	/*SetOutline(true);*/
	return S_OK;
}

void CCoreMinotaur::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTranslate(GetModel()->GetCenterPos());
		Containers.second->SetScaleToFitModel(GetModel()->GetMinVertexPos(), GetModel()->GetMaxVertexPos());
		Containers.second->SetTransform(GetTransform());

	}
}


void CCoreMinotaur::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
	//for (auto& Colliders : GetColliderContainer())
	//	Colliders.second->AddRenderer(RENDERID::RI_NONALPHA_LAST);
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

