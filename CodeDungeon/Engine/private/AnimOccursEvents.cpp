#include "EngineDefine.h"
#include "UAnimOccursEvent.h"
#include "UMethod.h"
#include "UAnimModel.h"

UAnimOccurEvent::UAnimOccurEvent() :
	UAnimEvent(ANIMEVENTCATEGORY::CATEGROY_SECTION), m_AnimOccurDesc{}
{
}

UAnimOccurEvent::UAnimOccurEvent(std::ifstream& _load) :
	UAnimEvent(ANIMEVENTCATEGORY::CATEGROY_SECTION), m_AnimOccurDesc{}
{
	LoadEvent(_load);
}

ANIMEVENTDESC* const UAnimOccurEvent::OutAnimEventDesc()
{
	return &m_AnimOccurDesc;
 }
void UAnimOccurEvent::ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc)
{
	assert(nullptr != _AnimEventDesc);
	m_AnimOccurDesc = *static_cast<ANIMOCURRESDESC*>(_AnimEventDesc);
 }

ANIMOTHEREVENTDESC* const UAnimOccurEvent::OutOtherEventDesc()
{
	return &m_AnimChangeDesc;
}

void UAnimOccurEvent::ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc)
{
	assert(nullptr != _AnimOtherEventDesc);
	m_AnimChangeDesc = *static_cast<ANIMCHANGEDESC*>(_AnimOtherEventDesc);
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
	}
	return m_AnimOccurDesc.dAnimOccursTime;
}

void UAnimOccurEvent::EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta)
{
	_pAnimModel->SetSupplyLerpValue(m_AnimChangeDesc.fSupplyAnimValue);
	_pAnimModel->ChangeAnimation(m_AnimChangeDesc.iNextAnimIndex, m_AnimChangeDesc.dNextAnimTimeAcc);
}

void UAnimOccurEvent::SaveEvent(std::ofstream& _save)
{
	UMethod::SaveString(_save, m_AnimOccurDesc.wstrEventTrigger);
	_save.write((_char*)&m_AnimOccurDesc.dAnimOccursTime, sizeof(_double));
	_save.write((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimOccurEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
	UMethod::ReadString(_load, m_AnimOccurDesc.wstrEventTrigger);
	_load.read((_char*)&m_AnimOccurDesc.dAnimOccursTime, sizeof(_double));
	_load.read((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimOccurEvent::Free()
{
}
