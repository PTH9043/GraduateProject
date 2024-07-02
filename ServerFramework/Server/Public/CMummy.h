#pragma once
#include "AGameObject.h"


BEGIN(Server)

class CMummy : public AGameObject{
public:
	CMummy(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CMummy)
public:
	virtual _bool Start() override;
	virtual void Tick() override;
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
private:
	virtual void Free() override;
private:


};

END

