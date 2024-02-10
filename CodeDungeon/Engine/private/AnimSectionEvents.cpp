#include "EngineDefine.h"
#include "AnimSectionEvents.h"
#include "UMethod.h"

UAnimChangeBetweenEvent::UAnimChangeBetweenEvent() : 
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN)
{
}

UAnimChangeBetweenEvent::UAnimChangeBetweenEvent(std::ifstream& _load) :
	UAnimSectionEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN)
{ 
	LoadEvent(_load);
}

UAnimChangeBetweenEvent::UAnimChangeBetweenEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc) : 
	UAnimSectionEvent(_AnimEventDesc, ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN)
{
}

ANIMOTHEREVENTDESC* const UAnimChangeBetweenEvent::OutOtherEventDesc()
{
	return &m_ACBEventDesc;
}

void UAnimChangeBetweenEvent::ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc)
{
	RETURN_CHECK(nullptr == _AnimOtherEventDesc, ;);
	m_ACBEventDesc = *static_cast<ANIMCHANGEBETWEENEVENTDESC*>(_AnimOtherEventDesc);
}

void UAnimChangeBetweenEvent::EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta)
{
}

void UAnimChangeBetweenEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);

}

void UAnimChangeBetweenEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
}

void UAnimChangeBetweenEvent::Free()
{
}
