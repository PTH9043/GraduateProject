#pragma once
#include "AMonster.h"

BEGIN(Server)

class CMummy final : public AMonster {
public:
	CMummy(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CMummy)
public:
	virtual _bool Start() override;
	virtual void State() override;
private:
	virtual void Free() override;
private:

};

END

