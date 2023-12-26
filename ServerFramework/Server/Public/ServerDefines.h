#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES
#define _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES

#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

#include "CoreDefines.h"
#include "ProtocolBuffer.h"

#ifdef _DEBUG
#pragma comment(lib, "CoreD")
#pragma comment(lib, "ProtocolBufferD")
#else
#pragma comment(lib, "Core")
#pragma comment(lib, "ProtocolBuffer")
#endif

namespace Server{}
namespace Core{}

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES