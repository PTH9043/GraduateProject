#include "CoreDefines.h"
#include "CoreGrobal.h"
#include "UDeadLockProfiler.h"
#include "Memory.h"

namespace Core
{
#ifdef USE_DEBUG
	CORE_DLL UDeadLockProfiler* g_DeadLockProfiler = nullptr;
#endif

	CORE_DLL  UMemoryAdminster* g_MemoryAdminster = nullptr;

	class CoreGlobal
	{
	public:
		CoreGlobal()
		{
#ifdef USE_DEBUG
			g_DeadLockProfiler = new UDeadLockProfiler;
#endif

			g_MemoryAdminster = new UMemoryAdminster;
		}

		~CoreGlobal()
		{
#ifdef USE_DEBUG
			delete g_DeadLockProfiler;
#endif
			delete g_MemoryAdminster;
		}
	} CoreGlobal;
} 
