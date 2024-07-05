#include "ServerDefines.h"
#include "CMummy.h"

namespace Server
{
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID)
		: AMonster(OBJCON_CONDATA, _ID)
	{
	}
	_bool CMummy::Start()
	{
		CreateAnimator("..\\..\\Resource\\Anim\\Mummy\\", "Mummy_DEMO_1_FBX.bin");
		return __super::Start();
	}

	void CMummy::State()
	{
	}

	void CMummy::Free()
	{
	}

}