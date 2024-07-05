#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMAINLOOP_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMAINLOOP_H

#include "ACoreObject.h"

BEGIN(Core)
class AServerService;
class AGameTimer;

class AMainLoop final : public ACoreObject {
public:
	AMainLoop(OBJCON_CONSTRUCTOR, Asio::io_context& _context);
	DESTRUCTOR(AMainLoop)
public:
	void RegisterTimer(const _double& _dTimeDelta);
	void TimerThread(const boost::system::error_code& _error);
private:
	virtual void Free() override;
private:
	Asio::deadline_timer 				m_DeadLineEvent;

	SHPTR<AGameTimer>			m_spGameTimer;
	WKPTR< AServerService>		m_wpServerService;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMAINLOOP_H