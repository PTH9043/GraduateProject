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


CItemChest::CItemChest(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType)
{
}

CItemChest::CItemChest(const CItemChest& _rhs)
	: CMob(_rhs)
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
	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTranslate(GetAnimModel()->GetCenterPos());
		Containers.second->SetScaleToFitModel(GetAnimModel()->GetMinVertexPos(), GetAnimModel()->GetMaxVertexPos());
	}
	
	return S_OK;
}

void CItemChest::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	static _double elapsedTime = 0;
	_double ItemChestOpeningSpeed = 2;
	_double ItemChestTimeArcOpenEnd = 3;
	GetAnimationController()->Tick(_dTimeDelta);

	//상자 여는 트리거
	if (GetFoundTargetState())
	{
		if (spGameInstance->GetDIKeyDown(DIK_F))
			SetOpeningState(true);
	}

	if (m_bisOpen)
	{
		SetElapsedTime(GetElapsedTime() + _dTimeDelta * ItemChestOpeningSpeed);
		if (GetElapsedTime() < ItemChestTimeArcOpenEnd)
			GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, GetElapsedTime());
	}

	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTransform(GetTransform());
	}
}

void CItemChest::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
	/*for (auto& Colliders : GetColliderContainer())
		Colliders.second->AddRenderer(RENDERID::RI_NONALPHA_LAST);*/
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
