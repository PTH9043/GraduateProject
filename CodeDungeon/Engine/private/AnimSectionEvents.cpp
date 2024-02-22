#include "EngineDefine.h"
#include "AnimSectionEvents.h"
#include "UMethod.h"
#include "UAnimModel.h"
#include "UCollider.h"
#include "UBoneNode.h"
#include "UGameInstance.h"

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

void UAnimChangeBetweenEvent::EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta)
{
	_pAnimModel->SetSupplyLerpValue(m_AnimChangeDesc.fSupplyAnimValue);
	_pAnimModel->ChangeAnimation(m_AnimChangeDesc.iNextAnimIndex, m_AnimChangeDesc.dNextAnimTimeAcc);
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

_bool UAnimColliderEvent::EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc, const _wstring& _wstrInputTrigger)
{
#ifdef _USE_DEBUGGING
	if (nullptr != m_AnimColliderDesc.spCollider)
	{
		m_AnimColliderDesc.spCollider->AddRenderer(RENDERID::RI_NONALPHA_LAST);
	}
#endif
	return __super::EventCheck(_pAnimModel, _dTimeAcc, _dTimeAcc, _wstrInputTrigger);
}

const ANIMOTHEREVENTDESC*  UAnimColliderEvent::OutOtherEventDesc()
{
	return &m_AnimColliderDesc;
}

void UAnimColliderEvent::EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta)
{
}

void UAnimColliderEvent::SaveEvent(std::ofstream& _save)
{
	assert(nullptr != m_AnimColliderDesc.spBoneNode && nullptr != m_AnimColliderDesc.spCollider);

	__super::SaveEvent(_save);
	UMethod::SaveString(_save, m_AnimColliderDesc.wstrBoneName);
	_save.write((_char*)&m_AnimColliderDesc.iColliderType, sizeof(_int));

	UCollider::COLLIDERDESC Desc(m_AnimColliderDesc.spCollider->GetScale(), m_AnimColliderDesc.spCollider->GetTranslate());
	_save.write((_char*)&Desc, sizeof(UCollider::COLLIDERDESC));
}

void UAnimColliderEvent::LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load)
{
	__super::LoadEvent(_spAnimModel, _load);
	UMethod::ReadString(_load, m_AnimColliderDesc.wstrBoneName);
	_load.read((_char*)&m_AnimColliderDesc.iColliderType, sizeof(_int));

	UCollider::COLLIDERDESC Desc;
	_load.read((_char*)&Desc, sizeof(UCollider::COLLIDERDESC));

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_AnimColliderDesc.spBoneNode = _spAnimModel->FindBoneNode(m_AnimColliderDesc.wstrBoneName);
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

	assert(nullptr != m_AnimColliderDesc.spBoneNode && nullptr != m_AnimColliderDesc.spCollider);
}

void UAnimColliderEvent::Free()
{
}
