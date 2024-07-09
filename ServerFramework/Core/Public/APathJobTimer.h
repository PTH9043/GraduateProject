#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASTARJOBTIMER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASTARJOBTIMER_H

#include "AJobTimer.h"

BEGIN(Core)
class ANavigation;
class APathFinder;

class CORE_DLL APathJobTimer : public AJobTimer {
public:
	APathJobTimer(OBJCON_CONSTRUCTOR, Asio::io_context& _context);
	DESTRUCTOR(APathJobTimer)
public:
protected:
	virtual void TickTimer(const TIMEREVENT& _TimerEvent) override;
private:
	virtual void Free() override;
private:
	SHPTR<ANavigation>		m_spNavigation;
	SHPTR<APathFinder>		m_spPathFinder;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASTARJOBTIMER_H