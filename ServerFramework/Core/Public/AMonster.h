#pragma once
#include "AGameObject.h"

BEGIN(Core)

class AMonster abstract : public AGameObject{
public:
	AMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(AMonster)
public:

private:
	virtual void Free() override;
private:

};


END