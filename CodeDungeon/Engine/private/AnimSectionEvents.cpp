#include "EngineDefine.h"
#include "AnimSectionEvents.h"
#include "UMethod.h"
#include "UAnimModel.h"
#include "UCollider.h"
#include "UBoneNode.h"
#include "UGameInstance.h"
#include "UPawn.h"
#include "UTransform.h"
#include "UAnimation.h"

UAnimChangeBetweenEvent::UAnimChangeBetweenEvent() : 
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN)
{
}

UAnimChangeBetweenEvent::UAnimChangeBetweenEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) :
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN)
{ 
	LoadEvent(_spAnimModel, _load);
}

const ANIMOTHEREVENTDESC*  UAnimChangeBetweenEvent::OutOtherEventDesc()
{
	return &m_AnimChangeDesc;
}

void UAnimChangeBetweenEvent::EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	// 만약 EnableLastLerp가 켜져 있지 않다면
	if (false == m_AnimChangeDesc.isEnableLastSettingAnim)
	{
		// 바꿔야 하는 구간에 바꾼다. 
		if (m_AnimChangeDesc.fAnimChangeTime <= _dTimeAcc)
		{
			ResetEventData(_pAnimModel);
			// 현재 event 비활성화
			_pAnimModel->SetSupplyLerpValue(m_AnimChangeDesc.fSupplyAnimValue);
			_pAnimModel->ChangeAnimation(m_AnimChangeDesc.iNextAnimIndex, m_AnimChangeDesc.dNextAnimTimeAcc);
		}
	}
	// 켜저 있다면
	else
	{
		// 만약
		if (_pAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= m_AnimChangeDesc.fLastProgressValue)
		{
			ResetEventData(_pAnimModel);
			_pAnimModel->SetAnimation(m_AnimChangeDesc.iNextAnimIndex);
		}
	}
}

void UAnimChangeBetweenEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	_save.write((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimChangeBetweenEvent::LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load)
{
	__super::LoadEvent(_spAnimModel, _load);
	_load.read((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimChangeBetweenEvent::Free()
{
}

/*
=================================================
AnimChangeBetweenEvent
=================================================
AnimColliderEvent
=================================================
*/

UAnimColliderEvent::UAnimColliderEvent() : 
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_COLLIDER), m_AnimColliderDesc{}
{

}

UAnimColliderEvent::UAnimColliderEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) :
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_COLLIDER), m_AnimColliderDesc{}
{
	LoadEvent(_spAnimModel, _load);
}

_bool UAnimColliderEvent::EventCheck(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc, const _wstring& _wstrInputTrigger)
{
#ifdef _USE_DEBUGGING
	if (nullptr != m_AnimColliderDesc.spCollider)
	{
		SHPTR<UTransform> spTransform = _pPawn->GetTransform();
		if (nullptr != m_AnimColliderDesc.spBoneNode){
			m_AnimColliderDesc.spCollider->SetTransform(m_AnimColliderDesc.spBoneNode->GetCombineMatrix() * spTransform->GetWorldMatrix());
		}
		else
		{
			m_AnimColliderDesc.spCollider->SetTransform(spTransform->GetWorldMatrix());
		}
		m_AnimColliderDesc.spCollider->AddRenderer(RENDERID::RI_NONALPHA_LAST);
		m_AnimColliderDesc.spCollider->ChangeColliderColor(_float3{ 0.f, 1.f, 1.f });
	}
#endif
	return __super::EventCheck(_pPawn, _pAnimModel, _dTimeAcc, _dTimeAcc, _wstrInputTrigger);
}

const ANIMOTHEREVENTDESC*  UAnimColliderEvent::OutOtherEventDesc()
{
	return &m_AnimColliderDesc;
}

void UAnimColliderEvent::EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
#ifdef _USE_DEBUGGING
	if (nullptr != m_AnimColliderDesc.spCollider) {
		m_AnimColliderDesc.spCollider->ChangeColliderColor(_float3{ 1.f, 1.f, 0.f });
	}
#endif
}

void UAnimColliderEvent::SaveEvent(std::ofstream& _save)
{
	assert(nullptr != m_AnimColliderDesc.spCollider);

	__super::SaveEvent(_save);
	if (nullptr == m_AnimColliderDesc.spBoneNode)
	{
		UMethod::SaveString(_save, L"");
	}
	else
	{
		UMethod::SaveString(_save, m_AnimColliderDesc.spBoneNode->GetName());
	}
	_save.write((_char*)&m_AnimColliderDesc.iColliderType, sizeof(_int));

	UCollider::COLLIDERDESC Desc(m_AnimColliderDesc.spCollider->GetScale(), m_AnimColliderDesc.spCollider->GetTranslate());
	_save.write((_char*)&Desc, sizeof(UCollider::COLLIDERDESC));
}

void UAnimColliderEvent::LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load)
{
	__super::LoadEvent(_spAnimModel, _load);
	_wstring wstrBoneName;
	UMethod::ReadString(_load, wstrBoneName);
	_load.read((_char*)&m_AnimColliderDesc.iColliderType, sizeof(_int));

	UCollider::COLLIDERDESC Desc;
	_load.read((_char*)&Desc, sizeof(UCollider::COLLIDERDESC));

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_AnimColliderDesc.spBoneNode = _spAnimModel->FindBoneNode(wstrBoneName);
	switch (m_AnimColliderDesc.iColliderType)
	{
	case UCollider::TYPE_AABB:
		m_AnimColliderDesc.spCollider = std::static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_ABBCOLLIDER, VOIDDATAS{ &Desc }));
		break;
	case UCollider::TYPE_OBB:
		m_AnimColliderDesc.spCollider = std::static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, VOIDDATAS{ &Desc }));
		break;
	case UCollider::TYPE_SPHERE:
		m_AnimColliderDesc.spCollider = std::static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_SPHERECOLLIDER, VOIDDATAS{ &Desc }));
		break;
	}

	assert( nullptr != m_AnimColliderDesc.spCollider);
}

void UAnimColliderEvent::Free()
{
	m_AnimColliderDesc.spCollider.reset();
}
