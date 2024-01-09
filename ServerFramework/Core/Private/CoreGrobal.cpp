#include "CoreDefines.h"
#include "CoreGrobal.h"
#include "UDeadLockProfiler.h"
#include "Memory.h"
#include "UApp.h"

namespace Core
{
#ifdef USE_DEBUG
	CORE_DLL UDeadLockProfiler*		g_pDeadLockProfiler = nullptr;
#endif

	CORE_DLL  UMemoryAdminster* g_pMemoryAdminster = nullptr;
	CORE_DLL	 UApp*						    g_RegisterApp = nullptr;

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
			delete g_RegisterApp;

#ifdef USE_DEBUG
			delete g_pDeadLockProfiler;
#endif
			delete g_pMemoryAdminster;
		}
	};
	CoreGlobal CoreGrobal;

} 
