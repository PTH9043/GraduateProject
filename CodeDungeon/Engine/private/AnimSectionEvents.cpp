#include "EngineDefine.h"
#include "UAnimSectionEvent.h"
#include "UMethod.h"
#include "UAnimModel.h"

UAnimSectionEvent::UAnimSectionEvent() :
	UAnimEvent(ANIMEVENTCATEGORY::CATEGROY_SECTION), m_AnimSectionDesc{}
{

}

UAnimSectionEvent::UAnimSectionEvent(std::ifstream& _load) :
	UAnimEvent(ANIMEVENTCATEGORY::CATEGROY_SECTION), m_AnimSectionDesc{}
{
	LoadEvent(_load);
}

_bool UAnimSectionEvent::EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
	const _wstring& _wstrInputTrigger)
{
	if (m_AnimSectionDesc.IsAnimEventActive(_dTimeAcc))
	{
		if (m_AnimSectionDesc.wstrEventTrigger == _wstrInputTrigger)
		{
			EventSituation(_pAnimModel, _dTimeDelta);
			return true;
		}
	}
	return false;
}

void UAnimSectionEvent::ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc)
{
	RETURN_CHECK(nullptr == _AnimEventDesc, ;);
	m_AnimSectionDesc = *static_cast<ANIMEVENTSECTIONDESC*>(_AnimEventDesc);
}

ANIMOTHEREVENTDESC* const UAnimSectionEvent::OutOtherEventDesc()
{
	return &m_AnimChangeDesc;
}

void UAnimSectionEvent::ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc)
{
	assert(nullptr != _AnimOtherEventDesc);
	m_AnimChangeDesc = *static_cast<ANIMCHANGEDESC*>(_AnimOtherEventDesc);
}

void UAnimSectionEvent::EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta)
{
	_pAnimModel->SetSupplyLerpValue(m_AnimChangeDesc.fSupplyAnimValue);
	_pAnimModel->ChangeAnimation(m_AnimChangeDesc.iNextAnimIndex, m_AnimChangeDesc.dNextAnimTimeAcc);
}

void UAnimSectionEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	UMethod::SaveString(_save, m_AnimSectionDesc.wstrEventTrigger);
	_save.write((_char*)&m_AnimSectionDesc.dStartTime, sizeof(_double));
	_save.write((_char*)&m_AnimSectionDesc.dEndTime, sizeof(_double));
	_save.write((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimSectionEvent::LoadEvent(std::ifstream& _load)
{
	__super::LoadEvent(_load);
	UMethod::ReadString(_load, m_AnimSectionDesc.wstrEventTrigger);
	_load.read((_char*)&m_AnimSectionDesc.dStartTime, sizeof(_double));
	_load.read((_char*)&m_AnimSectionDesc.dEndTime, sizeof(_double));
	_load.read((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimSectionEvent::Free()
{
}
