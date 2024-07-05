#include "ServerDefines.h"
#include "CMonsterJobTimer.h"

namespace Server
{
	CMonsterJobTimer::CMonsterJobTimer(OBJCON_CONSTRUCTOR, Asio::io_context& _context) :
		AJobTimer(OBJCON_CONDATA, _context)
	{
	}

	void CMonsterJobTimer::TickTimer(const TIMEREVENT& _TimerEvent)
	{
		switch (_TimerEvent.eEventType)
		{
		case EV_MOB_FIND:

			break;
		case EV_MOB_ATTACK:

			break;
		}
	}

	void CMonsterJobTimer::Free()
	{
	}
}