#include "EngineDefine.h"
#include "AnimSectionEvents.h"
#include "UMethod.h"
#include "UAnimModel.h"

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
	return &m_AnimChangeDesc;
}

void UAnimChangeBetweenEvent::ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc)
{
	assert(nullptr != _AnimOtherEventDesc);
	m_AnimChangeDesc = *static_cast<ANIMCHANGEDESC*>(_AnimOtherEventDesc);
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

void UAnimChangeBetweenEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
	_load.read((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimChangeBetweenEvent::Free()
{
}
