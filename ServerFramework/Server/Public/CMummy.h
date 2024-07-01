#pragma once
#include "AGameObject.h"


BEGIN(Server)

class CMummy : public AGameObject{
public:
	CMummy(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(CMummy)
public:
	virtual _bool Start() override;

private:
	virtual void Free() override;
private:

};

END

