#include "EngineDefine.h"
#include "AnimOccursEvents.h"
#include "UMethod.h"
#include "UAnimModel.h"
#include "UGameInstance.h"
#include "USound.h"
#include "UCharacter.h"
#include "UPawn.h"

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

void UAnimOccursTimePassEvent::EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta)
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

/*
=================================================
UAnimOccursTimePassEvent
=================================================
AnomSoundEvent
=================================================
*/

UAnimSoundEvent::UAnimSoundEvent() :
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_SOUND), m_spSound{nullptr}
{
}

UAnimSoundEvent::UAnimSoundEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) :
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_SOUND), m_spSound{ nullptr }
{
	LoadEvent(_spAnimModel, _load);
}

const ANIMOTHEREVENTDESC* UAnimSoundEvent::OutOtherEventDesc()
{
	return &m_AnimSoundDesc;
}

void UAnimSoundEvent::EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta)
{
	if (nullptr == m_spSound)
	{
		RETURN_CHECK(true == m_AnimSoundDesc.wstrSoundName.empty(), ;);
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		m_spSound = spGameInstance->BringSound(m_AnimSoundDesc.wstrSoundName);
		m_spSound->ChangeMinMaxDistance3D(m_AnimSoundDesc.fMinSoundDistance, m_AnimSoundDesc.fMaxSoundDistance);
		m_spPlayerCharacter = spGameInstance->GetCurrPlayer();
	}
	m_spSound->Tick();
	if (false == m_spSound->IsSoundPlay()){
		m_spSound->Play();
	}
	if (nullptr == m_spPlayerCharacter)
		m_spSound->UpdateSound3D(_pPawn->GetTransform(), m_AnimSoundDesc.vSoundVelocity);
	else
		m_spSound->UpdateSound3D(_pPawn->GetTransform(), m_AnimSoundDesc.vSoundVelocity, m_spPlayerCharacter->GetTransform());
}

void UAnimSoundEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	UMethod::SaveString(_save, m_AnimSoundDesc.wstrSoundName);
	UMethod::SaveOther(_save, &m_AnimSoundDesc.vSoundVelocity);
	UMethod::SaveOther(_save, &m_AnimSoundDesc.fMaxSoundDistance);
	UMethod::SaveOther(_save, &m_AnimSoundDesc.fMinSoundDistance);
}

void UAnimSoundEvent::LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load)
{
	__super::LoadEvent(_spAnimModel, _load);
	UMethod::ReadString(_load, m_AnimSoundDesc.wstrSoundName);
	UMethod::ReadOther(_load, &m_AnimSoundDesc.vSoundVelocity);
	UMethod::ReadOther(_load, &m_AnimSoundDesc.fMaxSoundDistance);
	UMethod::ReadOther(_load, &m_AnimSoundDesc.fMinSoundDistance);
}

void UAnimSoundEvent::Free()
{
}
