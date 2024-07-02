#include "CoreDefines.h"
#include "AJobTimer.h"
#include <functional>

namespace Core {
	AJobTimer::AJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service) :
		ACoreObject(OBJCON_CONDATA),
		m_SteadyEvent{_service }
	{
	}

	void AJobTimer::RegisterTimer(_int _RegisterTimer)
	{
		m_SteadyEvent.expires_from_now(std::chrono::milliseconds(_RegisterTimer));
		// 타이머의 비동기 대기 설정
		m_SteadyEvent.async_wait(std::bind(&AJobTimer::TimerThread, this, std::placeholders::_1));
	}

	void AJobTimer::TimerThread(const boost::system::error_code& _error)
	{
		TIMEREVENT TimerEvent;
		auto CurrentTime = std::chrono::system_clock::now();

		if (true == m_TimerEventQueue.try_pop(TimerEvent))
		{
			if (TimerEvent.WakeUpTime > CurrentTime) {
				auto MTime = TimerEvent.WakeUpTime - CurrentTime;
				std::this_thread::sleep_for(MTime);
			}
			TickTimer(TimerEvent);
		}
		RegisterTimer(1);
	}


	void AJobTimer::Free()
	{
	}
}