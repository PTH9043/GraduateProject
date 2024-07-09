#include "CoreDefines.h"
#include "APathJobTimer.h"
#include "ACoreInstance.h"
#include "ANavigation.h"
#include "APathFinder.h"
#include "AGameObject.h"
#include "ASession.h"
#include "AMonster.h"
#include "ATransform.h"
#include "ACell.h"

namespace Core
{
	APathJobTimer::APathJobTimer(OBJCON_CONSTRUCTOR, Asio::io_context& _context) :
		AJobTimer(OBJCON_CONDATA, _context), m_spNavigation{nullptr}
	{
		// Astar Àü¿ë Navigation Index
		m_spNavigation = GetCoreInstance()->GetNavigation(ASTAR_NAV_INDEX);
		m_spPathFinder = CreateInitNative<APathFinder>(m_spNavigation);
	}

	void APathJobTimer::Free()
	{
	}

	void APathJobTimer::TickTimer(const TIMEREVENT& _TimerEvent)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		
		SHPTR<AMonster> spMonster = spCoreInstance->FindMobObject(_TimerEvent.llObjID);
		SHPTR<ASession> spSession = spCoreInstance->FindSession(_TimerEvent.llTargetID);

		SHPTR<ATransform> spMonsterTr = spMonster->GetTransform();
		SHPTR<ATransform> spSessionTr = spSession->GetTransform();

		LIST<SHPTR<ACell>> PathList = m_spPathFinder->FindPath(spMonsterTr, spSessionTr);
		spMonster->InsertPathList(PathList);
	}
}