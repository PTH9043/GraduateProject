#include "ServerDefines.h"
#include "CMonsterJobTimer.h"

namespace Server
{
	CMonsterJobTimer::CMonsterJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service,
		_int _millisecondsTimer) : 
		AJobTimer(OBJCON_CONDATA, _service, _millisecondsTimer)
	{
	}

	void CMonsterJobTimer::TickTimer()
	{
	}

	void CMonsterJobTimer::Free()
	{
	}
}