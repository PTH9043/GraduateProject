#include "EngineDefine.h"
#include "AnimEventParents.h"
#include "UMethod.h"

UAnimEvent::UAnimEvent(ANIMEVENTTYPE _AnimEventType, ANIMEVENTCATEGORY _AnimEventCategory) :
	m_AnimEventType{ _AnimEventType },
	m_AnimEventCategory{_AnimEventCategory}
{
}

void UAnimEvent::SaveEvent( std::ofstream& _save)
{
	_save.write((_char*)&m_AnimEventType, sizeof(ANIMEVENTTYPE));
}

void UAnimEvent::LoadEvent( std::ifstream& _load)
{
	_load.read((_char*)&m_AnimEventType, sizeof(ANIMEVENTTYPE));
}

void UAnimEvent::Free()
{
}

/*
=====================================
AnimEvent
=====================================
AnimSectionEvent
=====================================
*/

UAnimSectionEvent::UAnimSectionEvent(ANIMEVENTTYPE _AnimEventType) : 
	UAnimEvent(_AnimEventType, ANIMEVENTCATEGORY::CATEGROY_SECTION), m_AnimSectionDesc{}
{

}

UAnimSectionEvent::UAnimSectionEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc, ANIMEVENTTYPE _AnimEventType) :
	m_AnimSectionDesc{ _AnimEventDesc }, UAnimEvent(_AnimEventType, ANIMEVENTCATEGORY::CATEGROY_SECTION)
{

}


_bool UAnimSectionEvent::EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
	const _wstring& _wstrInputTrigger)
{
	if (m_AnimSectionDesc.IsAnimEventActive(_dTimeAcc))
	{
		if (m_AnimSectionDesc.wstrEventTrigger == _wstrInputTrigger)
		{
			EventSituation(_pAnimModel, _dTimeDelta);
		}
		return true;
	}
	return false;
}

void UAnimSectionEvent::ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc)
{
	RETURN_CHECK(nullptr == _AnimEventDesc, ;);
	m_AnimSectionDesc = *static_cast<ANIMEVENTSECTIONDESC*>(_AnimEventDesc);
}

void UAnimSectionEvent::SaveEvent( std::ofstream& _save)
{
	__super::SaveEvent(_save);
	UMethod::SaveString(_save, m_AnimSectionDesc.wstrEventTrigger);
	_save.write((_char*)&m_AnimSectionDesc.dStartTime, sizeof(_double));
	_save.write((_char*)&m_AnimSectionDesc.dEndTime, sizeof(_double));
}

void UAnimSectionEvent::LoadEvent( std::ifstream& _load)
{
	__super::LoadEvent(_load);
	UMethod::ReadString(_load, m_AnimSectionDesc.wstrEventTrigger);
	_load.read((_char*)&m_AnimSectionDesc.dStartTime, sizeof(_double));
	_load.read((_char*)&m_AnimSectionDesc.dEndTime, sizeof(_double));
}

void UAnimSectionEvent::Free()
{
}

/*
=====================================
AnimSectionEvent
=====================================
AnimOccurEvent
=====================================
*/

UAnimOccurEvent::UAnimOccurEvent(ANIMEVENTTYPE _AnimEventType) :
	UAnimEvent(_AnimEventType, ANIMEVENTCATEGORY::CATEGROY_SECTION), m_AnimOccurDesc{}
{
}

UAnimOccurEvent::UAnimOccurEvent(const ANIMOCURRESDESC& _AnimEventDesc, ANIMEVENTTYPE _AnimEventType) :
	m_AnimOccurDesc{ _AnimEventDesc }, UAnimEvent(_AnimEventType, ANIMEVENTCATEGORY::CATEGROY_OCCUR)
{
}


_bool UAnimOccurEvent::EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
	const _wstring& _wstrInputTrigger)
{
	if (m_AnimOccurDesc.IsAnimOcurrs(_dTimeAcc))
	{
		if (m_AnimOccurDesc.wstrEventTrigger == _wstrInputTrigger)
		{
			EventSituation(_pAnimModel, _dTimeDelta);
		}
		return true;
	}
	return false;
}

void UAnimOccurEvent::ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc)
{
	RETURN_CHECK(nullptr == _AnimEventDesc, ;);
	m_AnimOccurDesc = *static_cast<ANIMOCURRESDESC*>(_AnimEventDesc);
}

void UAnimOccurEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	UMethod::SaveString(_save, m_AnimOccurDesc.wstrEventTrigger);
	_save.write((_char*)&m_AnimOccurDesc.dAnimOccursTime, sizeof(_double));
}

void UAnimOccurEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
	UMethod::ReadString(_load, m_AnimOccurDesc.wstrEventTrigger);
	&_load.read((_char*)&m_AnimOccurDesc.dAnimOccursTime, sizeof(_double));
}

void UAnimOccurEvent::Free()
{
}
