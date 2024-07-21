#include "ClientDefines.h"
#include "CMob.h"
#include "CSarcophagus.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "CMummyAnimController.h"
#include "UMethod.h"
#include "UCollider.h"
#include "UProcessedData.h"

CSarcophagus::CSarcophagus(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType), m_SarcophagusType{}
{
}

CSarcophagus::CSarcophagus(const CSarcophagus& _rhs)
	: CMob(_rhs), m_SarcophagusType{}
{
}

void CSarcophagus::Free()
{
}

HRESULT CSarcophagus::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CSarcophagus::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	SetActivationRange(30);
	SetDeactivationRange(80);
	/*UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(1.f, 1.f, 1.f);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));
	_wstring mainColliderTag = L"Main";
	AddColliderInContainer(mainColliderTag, Collider);

	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTranslate(GetAnimModel()->GetCenterPos());
		Containers.second->SetScaleToFitModel(GetAnimModel()->GetMinVertexPos(), GetAnimModel()->GetMaxVertexPos());
	}*/

	return S_OK;
}

void CSarcophagus::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
{
#ifdef _ENABLE_PROTOBUFF

	__super::ReceiveNetworkProcessData(_ProcessData);

	switch (_ProcessData.GetDataType())
	{
	case TAG_SC_MONSTERFIND:
	{
		SetFoundTargetState(true);
	}
		break;
	}
#endif
}

void CSarcophagus::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	FindPlayer();

	_double SarcophagusOpeningSpeed = 20;
	_double LyingSarcophagusTimeArcOpenStart = 50;
	_double LyingSarcophagusTimeArcOpenEnd = 50;
	_double StandingSarcophagusTimeArcOpenEnd = 30;

	GetAnimationController()->Tick(_dTimeDelta);

	if (GetFoundTargetState())
	{
		SetElapsedTime(GetElapsedTime() + _dTimeDelta * SarcophagusOpeningSpeed);
	}
	if (GetSarcophagusType() == SARCOTYPE::TYPE_LYING)
	{
		if (GetElapsedTime() < LyingSarcophagusTimeArcOpenEnd)
		{
			GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, LyingSarcophagusTimeArcOpenStart + GetElapsedTime());
		}
	}
	else
		if (GetElapsedTime() < StandingSarcophagusTimeArcOpenEnd)
		{
			GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, GetElapsedTime());
		}

	for (auto& Containers : GetColliderContainer())
	{
		Containers.second->SetTransform(GetTransform());
	}
}

void CSarcophagus::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
}

HRESULT CSarcophagus::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CSarcophagus::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderShadowActive(_spCommand, _spTableDescriptor);
}

HRESULT CSarcophagus::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	return __super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
}

void CSarcophagus::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}
