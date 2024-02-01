#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H

#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

#include "CoreDefines.h"
#include "ProtocolBuffer.h"
#include "CoreSqlDefines.h"

#ifdef _DEBUG
#pragma comment(lib, "ProtocolBufferD")
#pragma comment(lib, "CoreD")
#else
#pragma comment(lib, "ProtocolBuffer")
#pragma comment(lib, "Core")
#endif
namespace Core {}

using namespace Core;

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H