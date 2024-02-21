#include "EngineDefine.h"
#include "AnimOccursEvents.h"
#include "UMethod.h"
#include "UAnimModel.h"

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent() : 
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS)
{
}

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent(std::ifstream& _load) :
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS)
{
	LoadEvent(_load);
}

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent(const ANIMOCURRESDESC& _AnimEventDesc) :
	UAnimOccurEvent(_AnimEventDesc, ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS)
{
}

ANIMOTHEREVENTDESC* const UAnimOccursTimePassEvent::OutOtherEventDesc()
{
	return &m_AnimChangeDesc;
}

void UAnimOccursTimePassEvent::ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc)
{
	assert(nullptr != _AnimOtherEventDesc);
	m_AnimChangeDesc = *static_cast<ANIMCHANGEDESC*>(_AnimOtherEventDesc);
}

void UAnimOccursTimePassEvent::EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta)
{
	_pAnimModel->SetSupplyLerpValue(m_AnimChangeDesc.fSupplyAnimValue);
	_pAnimModel->ChangeAnimation(m_AnimChangeDesc.iNextAnimIndex, m_AnimChangeDesc.dNextAnimTimeAcc);
}

void UAnimOccursTimePassEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	_save.write((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimOccursTimePassEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
	_load.read((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimOccursTimePassEvent::Free()
{
}
