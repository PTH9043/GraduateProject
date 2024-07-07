#include "ServerDefines.h"
#include "CMonsterJobTimer.h"
#include "AMonster.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"

namespace Server
{
	CMonsterJobTimer::CMonsterJobTimer(OBJCON_CONSTRUCTOR, Asio::io_context& _context) :
		AJobTimer(OBJCON_CONDATA, _context)
	{
	}

	void CMonsterJobTimer::TickTimer(const TIMEREVENT& _TimerEvent)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

		SHPTR<AMonster> spMonster = spCoreInstance->FindMobObject(_TimerEvent.llObjID);
		SHPTR<ASession> spPlayer = spCoreInstance->FindSession(_TimerEvent.llTargetID);
		
		spMonster->State(spPlayer, _TimerEvent.eEventType);
	}

	void CMonsterJobTimer::Free()
	{
	}
}