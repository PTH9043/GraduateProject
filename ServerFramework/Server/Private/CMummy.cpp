#include "ServerDefines.h"
#include "CMummy.h"

namespace Server
{
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID)
		: AGameObject(OBJCON_CONDATA, _ID, SESSIONTYPE::MONSTER)
	{
	}
	_bool CMummy::Start()
	{
		return _bool();
	}
	void CMummy::Tick()
	{
	}
	_bool CMummy::ProcessPacket(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		return _bool();
	}
	void CMummy::Free()
	{
	}
}