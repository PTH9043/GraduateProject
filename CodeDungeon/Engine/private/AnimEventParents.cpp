#include "EngineDefine.h"
#include "AnimEventParents.h"

UAnimEvent::UAnimEvent() : m_EventID{0}, m_AnimEventType{ANIMEVENT::EVENT_END}
{
}

UAnimEvent::UAnimEvent(const _int _EventID, ANIMEVENT _AnimEvent) :
	m_EventID{ _EventID },
	m_AnimEventType{ _AnimEvent }
{
}

void UAnimEvent::Save(const _wstring& _wstrPath)
{
	std::ofstream Save{ _wstrPath };
	RETURN_CHECK_CRASH(!Save, ; , "AnimEvent Save Failed  Plz Check Path");
	SaveEvent(Save);
}

void UAnimEvent::Load(const _wstring& _wstrPath)
{
	std::ifstream Load{ _wstrPath };
	RETURN_CHECK_CRASH(!Load, ;, "AnimEvent Load Failed  Plz Check Path");
	LoadEvent(Load);
}

void UAnimEvent::SaveEvent( std::ofstream& _save)
{
	_save.write((_char*)&m_EventID, sizeof(_int));
	_save.write((_char*)&m_AnimEventType, sizeof(ANIMEVENT));
}

void UAnimEvent::LoadEvent( std::ifstream& _load)
{
	_load.read((_char*)&m_EventID, sizeof(_int));
	_load.read((_char*)&m_AnimEventType, sizeof(ANIMEVENT));
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

UAnimSectionEvent::UAnimSectionEvent() : m_AnimEventDesc{}
{

}

UAnimSectionEvent::UAnimSectionEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc,
	const _int _EventID, ANIMEVENT _AnimEvent) :
	UAnimEvent(_EventID, _AnimEvent)
{
	::memcpy(&m_AnimEventDesc, &_AnimEvent, sizeof(ANIMEVENTSECTIONDESC));
}


_bool UAnimSectionEvent::EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	if (m_AnimEventDesc.IsAnimEventActive(_dTimeAcc))
	{
		EventSituation(_spAnimator, _dTimeDelta);
	}
	return m_AnimEventDesc.isEventActive;
}

void UAnimSectionEvent::SaveEvent( std::ofstream& _save)
{
	__super::SaveEvent(_save);
	_save.write((_char*)&m_AnimEventDesc, sizeof(ANIMEVENTSECTIONDESC));
}

void UAnimSectionEvent::LoadEvent( std::ifstream& _load)
{
	__super::LoadEvent(_load);
	_load.read((_char*)&m_AnimEventDesc, sizeof(ANIMEVENTSECTIONDESC));
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

UAnimOccurEvent::UAnimOccurEvent() : m_AnimOccurDesc{}
{
}

UAnimOccurEvent::UAnimOccurEvent(const ANIMOCURRESDESC& _AnimEventDesc, const _int _EventID, ANIMEVENT _AnimEvent) : 
	UAnimEvent(_EventID, _AnimEvent)
{
	::memcpy(&m_AnimOccurDesc, &_AnimEventDesc, sizeof(ANIMOCURRESDESC));
}


_bool UAnimOccurEvent::EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	if (m_AnimOccurDesc.IsAnimOcurrs(_dTimeAcc))
	{
		EventSituation(_spAnimator, _dTimeDelta);
	}
	return m_AnimOccurDesc.dAnimOccursTime;
}

void UAnimOccurEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	_save.write((_char*)&m_AnimOccurDesc, sizeof(ANIMOCURRESDESC));
}

void UAnimOccurEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
	_load.read((_char*)&m_AnimOccurDesc, sizeof(ANIMOCURRESDESC));
}

void UAnimOccurEvent::Free()
{
}
