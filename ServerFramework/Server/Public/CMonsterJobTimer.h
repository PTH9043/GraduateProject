#pragma once
#include "AJobTimer.h"


BEGIN(Server)

class CMonsterJobTimer final : public AJobTimer {
public:
	CMonsterJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service, _int _millisecondsTimer);
	DESTRUCTOR(CMonsterJobTimer)
protected:
	virtual void TickTimer() override;
private:
	virtual void Free() override;
private:

};

END