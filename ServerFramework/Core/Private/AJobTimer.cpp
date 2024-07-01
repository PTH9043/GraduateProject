#include "CoreDefines.h"
#include "AJobTimer.h"
#include <functional>

namespace Core {
	AJobTimer::AJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service, _int _millisecondsTimer) :
		ACoreObject(OBJCON_CONDATA),
		m_DeadLineTimer{_service, boost::posix_time::milliseconds(_millisecondsTimer)}
	{
	}

	void AJobTimer::RegisterAsio()
	{
		// 타이머의 비동기 대기 설정
		m_DeadLineTimer.async_wait(std::bind(&AJobTimer::TimerThread, this, std::placeholders::_1));
	}

	void AJobTimer::TimerThread(const boost::system::error_code& _error)
	{
		TickTimer();
	}

	void AJobTimer::TickTimer()
	{
	}

	void AJobTimer::Free()
	{
	}
}