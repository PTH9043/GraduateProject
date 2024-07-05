#include "CoreDefines.h"
#include "APawn.h"
#include "AAnimator.h"

namespace Core {

	APawn::APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		AGameObject(OBJCON_CONDATA, _ID, _SessionType)
	{
	}

	void APawn::AnimTick(const _double _dTimeDelta)
	{
		m_spAnimator->TickEvent(this, m_strAnimTrigger, _dTimeDelta);
		m_spAnimator->TickAnimChangeTransform(GetTransform(), _dTimeDelta);
	}

	void APawn::CreateAnimator(const _string& _strFolderPath, const _string& _strFileName)
	{
		m_spAnimator = Create<AAnimator>(GetCoreInstance(), _strFolderPath, _strFileName);
	}

	void APawn::SetAnimTrigger(const _string _strAnimTrigger)
	{
		WRITE_LOCK(m_TriggerLock);
		m_strAnimTrigger = _strAnimTrigger;
	}

	void APawn::Free()
	{
	}

}