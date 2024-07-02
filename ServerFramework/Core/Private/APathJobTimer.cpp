#include "CoreDefines.h"
#include "APathJobTimer.h"
#include "ACoreInstance.h"
#include "ANavigation.h"
#include "APathFinder.h"

namespace Core
{
	APathJobTimer::APathJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service) : 
		AJobTimer(OBJCON_CONDATA, _service), m_spNavigation{nullptr}
	{
		// Astar Àü¿ë Navigation Index
		m_spNavigation = GetCoreInstance()->GetNavigation(ASTAR_NAV_INDEX);
	}

	void APathJobTimer::TickTimer(const TIMEREVENT& _TimerEvent)
	{

	}

	void APathJobTimer::Free()
	{
	}
}