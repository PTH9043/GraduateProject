#include "EngineDefine.h"
#include "AnimOccursEvents.h"
#include "UMethod.h"
#include "UAnimModel.h"

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent() : 
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS)
{
}

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) :
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS)
{
	LoadEvent(_spAnimModel, _load);
}

const ANIMOTHEREVENTDESC*  UAnimOccursTimePassEvent::OutOtherEventDesc()
{
	return &m_AnimChangeDesc;
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

void UAnimOccursTimePassEvent::LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load)
{
	__super::LoadEvent(_spAnimModel, _load);
	_load.read((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimOccursTimePassEvent::Free()
{
}
