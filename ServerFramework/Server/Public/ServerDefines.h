#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H

#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

#include "CoreDefines.h"
#include "ProtocolBuffer.h"

#ifdef _DEBUG
#pragma comment(lib, "ProtocolBufferD")
#pragma comment(lib, "CoreD")
#else
#pragma comment(lib, "ProtocolBuffer")
#pragma comment(lib, "Core")
#endif

using namespace Core;

namespace Server 
{
	enum TIMERTYPE : Core::_int
	{
		TIMER_ASTAR, TIMER_DB, TIMER_MOB, TIMER_END
	};

	enum SARCOPHAGUSTYPE {
		SARCO_STANDING, SARCO_LAYING, SARCO_END
	};

	enum MUMMYTYPE {
		MUMMY_STANDING, MUMMY_LAYING, MUMMY_END
	};
}

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H