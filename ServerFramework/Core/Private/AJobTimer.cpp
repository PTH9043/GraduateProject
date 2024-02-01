#include "CoreDefines.h"
#include "AJobTimer.h"

namespace Core {
	AJobTimer::AJobTimer(OBJCON_CONSTRUCTOR) :
		ACoreObject(OBJCON_CONDATA), m_isRunningThread{true}
	{
	}

	void AJobTimer::TimerThread(void* _pJobTimer)
	{
		RETURN_CHECK(nullptr == _pJobTimer, ;);
		AJobTimer* pJobTimer = static_cast<AJobTimer*>(_pJobTimer);
		pJobTimer->RunTimer();
	}

	void AJobTimer::RunTimer()
	{
		while (m_isRunningThread) 
		{
			TickTimer();
		}
	}

	void AJobTimer::Free()
	{
	}
}