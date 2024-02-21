#include "EngineDefine.h"
#include "UAnimEvent.h"
#include "UMethod.h"
#include "UAnimSectionEvent.h"

UAnimEvent::UAnimEvent( ANIMEVENTCATEGORY _AnimEventCategory) :
	m_AnimEventCategory{_AnimEventCategory}
{
}

void UAnimEvent::SaveEvent( std::ofstream& _save)
{
	_save.write((_char*)&m_AnimEventCategory, sizeof(ANIMEVENTCATEGORY));
}

void UAnimEvent::LoadEvent( std::ifstream& _load)
{
	_load.read((_char*)&m_AnimEventCategory, sizeof(ANIMEVENTCATEGORY));
}

void UAnimEvent::Free()
{
}