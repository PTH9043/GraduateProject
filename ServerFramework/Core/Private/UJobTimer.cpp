#include "CoreDefines.h"
#include "UJobTimer.h"

namespace Core {
	UJobTimer::UJobTimer(OBJCON_CONSTRUCTOR) : 
		UObject(OBJCON_CONDATA), m_isRunningThread{true}
	{
	}

	void UJobTimer::TimerThread(void* _pJobTimer)
	{
		RETURN_CHECK(nullptr == _pJobTimer, ;);
		UJobTimer* pJobTimer = static_cast<UJobTimer*>(_pJobTimer);
		pJobTimer->RunTimer();
	}

	void UJobTimer::RunTimer()
	{
		while (m_isRunningThread) 
		{

		}
	}

	void UJobTimer::Free()
	{
	}
}