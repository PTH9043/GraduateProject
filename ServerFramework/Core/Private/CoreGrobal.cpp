#include "CoreDefines.h"
#include "CoreGrobal.h"
#include "UDeadLockProfiler.h"
#include "Memory.h"

namespace Core
{
#ifdef USE_DEBUG
	CORE_DLL UDeadLockProfiler* g_pDeadLockProfiler = nullptr;
#endif

	CORE_DLL  UMemoryAdminster* g_pMemoryAdminster = nullptr;

	class CoreGlobal
	{
	public:
		CoreGlobal()
		{
#ifdef USE_DEBUG
			g_pDeadLockProfiler = new UDeadLockProfiler;
#endif

			g_pMemoryAdminster = new UMemoryAdminster;
		}

		~CoreGlobal()
		{
#ifdef USE_DEBUG
			delete g_pDeadLockProfiler;
#endif
			delete g_pMemoryAdminster;
		}
	} CoreGlobal;
} 
