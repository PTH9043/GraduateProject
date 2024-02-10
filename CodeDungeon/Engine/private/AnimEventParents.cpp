#include "EngineDefine.h"
#include "AnimEventParents.h"

UAnimEvent::UAnimEvent(ANIMEVENTTYPE _AnimEventType, ANIMEVENTCATEGORY _AnimEventCategory) : 
m_AnimEventType{ _AnimEventType }, m_AnimEventCategory{ _AnimEventCategory }
{
}

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
	m_AnimSectionDesc.RegisterEventFunc();
}


_bool UAnimSectionEvent::EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	if (m_AnimSectionDesc.IsAnimEventActive(_dTimeAcc))
	{
		if (true == m_AnimSectionDesc.IsMousekeyboardFunc())
		{
			EventSituation(_spAnimator, _dTimeDelta);
		}
	}
	return m_AnimSectionDesc.isEventActive;
}

void UAnimSectionEvent::ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc)
{
	RETURN_CHECK(nullptr == _AnimEventDesc, ;);
	m_AnimSectionDesc = *static_cast<ANIMEVENTSECTIONDESC*>(_AnimEventDesc);
}

void UAnimSectionEvent::SaveEvent( std::ofstream& _save)
{
	__super::SaveEvent(_save);
	_save.write(m_AnimSectionDesc.SaveLoadPointer(), sizeof(ANIMEVENTSECTIONDESC)  - ANIMEVENTDESC::GetEventFuncSize());
}

void UAnimSectionEvent::LoadEvent( std::ifstream& _load)
{
	__super::LoadEvent(_load);
	_load.read(m_AnimSectionDesc.SaveLoadPointer(), sizeof(ANIMEVENTSECTIONDESC) - ANIMEVENTDESC::GetEventFuncSize());
	m_AnimSectionDesc.RegisterEventFunc();
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
	m_AnimOccurDesc.RegisterEventFunc();
}


_bool UAnimOccurEvent::EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	if (m_AnimOccurDesc.IsAnimOcurrs(_dTimeAcc))
	{
		if (true == m_AnimOccurDesc.IsMousekeyboardFunc())
		{
			EventSituation(_spAnimator, _dTimeDelta);
		}
	}
	return m_AnimOccurDesc.dAnimOccursTime;
}

void UAnimOccurEvent::ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc)
{
	RETURN_CHECK(nullptr == _AnimEventDesc, ;);
	m_AnimOccurDesc = *static_cast<ANIMOCURRESDESC*>(_AnimEventDesc);
}

void UAnimOccurEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	_save.write((_char*)&m_AnimOccurDesc, sizeof(ANIMOCURRESDESC) - ANIMEVENTDESC::GetEventFuncSize());
}

void UAnimOccurEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
	_load.read((_char*)&m_AnimOccurDesc, sizeof(ANIMOCURRESDESC) - ANIMEVENTDESC::GetEventFuncSize());
	m_AnimOccurDesc.RegisterEventFunc();
}

void UAnimOccurEvent::Free()
{
}
