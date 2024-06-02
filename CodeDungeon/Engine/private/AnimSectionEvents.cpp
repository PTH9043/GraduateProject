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
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN), 
	m_AnimChangeDesc{}
{
}

UAnimChangeBetweenEvent::UAnimChangeBetweenEvent(const UAnimChangeBetweenEvent& _rhs) : 
	UAnimSectionEvent(_rhs), m_AnimChangeDesc{_rhs.m_AnimChangeDesc }
{
}

UAnimChangeBetweenEvent::UAnimChangeBetweenEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) :
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN)
{ 
	LoadEvent(_spAnimModel, _load);
}

SHPTR<UAnimEvent> UAnimChangeBetweenEvent::Clone(UAnimModel* _pAnimModel)
{
	return std::move(CloneThis<UAnimChangeBetweenEvent>(*this));
}

const ANIMOTHEREVENTDESC*  UAnimChangeBetweenEvent::OutOtherEventDesc()
{
	return &m_AnimChangeDesc;
}

void UAnimChangeBetweenEvent::EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	RETURN_CHECK(_pAnimModel->GetCurrentAnimIndex() == m_AnimChangeDesc.iNextAnimIndex, ;);

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
			_pAnimModel->SetAnimation(m_AnimChangeDesc.iNextAnimIndex, m_AnimChangeDesc.dNextAnimTimeAcc);
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

UAnimColliderEvent::UAnimColliderEvent(const UAnimColliderEvent& _rhs) :
	UAnimSectionEvent(_rhs), m_AnimColliderDesc{ _rhs.m_AnimColliderDesc }
{

}

UAnimColliderEvent::UAnimColliderEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) :
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_COLLIDER), m_AnimColliderDesc{}
{
	LoadEvent(_spAnimModel, _load);
}

SHPTR<UAnimEvent> UAnimColliderEvent::Clone(UAnimModel* _pAnimModel)
{
	SHPTR<UAnimColliderEvent> spAnimEvent = CloneThis<UAnimColliderEvent>(*this);
	if (nullptr != _pAnimModel)
	{
		spAnimEvent->m_AnimColliderDesc.spCollider = _pAnimModel->BringAttackCollider(m_AnimColliderDesc.iColliderType);
	}
	return std::move(spAnimEvent);
}


_bool UAnimColliderEvent::EventCheck(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc, const _wstring& _wstrInputTrigger)
{
#ifdef _USE_DEBUGGING
	if (nullptr != m_AnimColliderDesc.spCollider)
	{
		SHPTR<UTransform> spTransform = _pPawn->GetTransform();

		m_AnimColliderDesc.spCollider->SetTranslate(m_AnimColliderDesc.vColliderTranslation);
		m_AnimColliderDesc.spCollider->SetScale(m_AnimColliderDesc.vColliderScale);

		if (nullptr != m_AnimColliderDesc.spBoneNode){
			m_AnimColliderDesc.spCollider->SetTransform(m_AnimColliderDesc.spBoneNode->GetCombineMatrix() * spTransform->GetWorldMatrix());
		}
		else
		{
			m_AnimColliderDesc.spCollider->SetTransform(spTransform->GetWorldMatrix());
		}
		m_AnimColliderDesc.spCollider->AddRenderer(RENDERID::RI_NONALPHA_LAST);
	}
#endif
	if (GetAnimSectionDesc().IsAnimEventActive(_dTimeAcc))
	{
		EventSituation(_pPawn, _pAnimModel, _dTimeDelta, _dTimeAcc);
		return true;
	}
	_pAnimModel->UpdateAttackData( false, m_AnimColliderDesc.spCollider);
	return false;
}

const ANIMOTHEREVENTDESC*  UAnimColliderEvent::OutOtherEventDesc()
{
	return &m_AnimColliderDesc;
}

void UAnimColliderEvent::EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	_pAnimModel->UpdateAttackData(true, m_AnimColliderDesc.spCollider);
}

void UAnimColliderEvent::SaveEvent(std::ofstream& _save)
{
	RETURN_CHECK(nullptr == m_AnimColliderDesc.spCollider, ;);

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

	m_AnimColliderDesc.vColliderScale = Desc.vScale;
	m_AnimColliderDesc.vColliderTranslation = Desc.vTranslation;
	m_AnimColliderDesc.spBoneNode = _spAnimModel->FindBoneNode(wstrBoneName);
}

void UAnimColliderEvent::Free()
{
	m_AnimColliderDesc.spCollider.reset();
}
