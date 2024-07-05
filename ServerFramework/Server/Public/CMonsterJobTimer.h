#pragma once
#include "AJobTimer.h"


BEGIN(Server)

class CMonsterJobTimer final : public AJobTimer {
public:
	CMonsterJobTimer(OBJCON_CONSTRUCTOR, Asio::io_context& _context);
	DESTRUCTOR(CMonsterJobTimer)
protected:
	virtual void TickTimer(const TIMEREVENT& _TimerEvent) override;
private:
	virtual void Free() override;
private:

};

END