#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREGROBAL_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREGROBAL_H

namespace Core
{
#ifdef USE_DEBUG
	CORE_DLL extern class UDeadLockProfiler* g_DeadLockProfiler;
#endif

	CORE_DLL 	extern class UMemoryAdminster* g_MemoryAdminster;
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREGROBAL_H