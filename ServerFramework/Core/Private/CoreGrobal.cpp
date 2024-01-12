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
			LOCKGUARD<MUTEX>	LL{ m_Mutex };
#ifdef USE_DEBUG
			g_pDeadLockProfiler = new UDeadLockProfiler;
#endif

			g_pMemoryAdminster = new UMemoryAdminster;
		}

		~CoreGlobal()
		{
			LOCKGUARD<MUTEX>	LL{ m_Mutex };
			// 만약 Register App이 비어있으면 크래쉬
			if (nullptr == g_RegisterApp)
			{
				CRASH("NOT REGISTER APP")
			}
			delete g_RegisterApp;

#ifdef USE_DEBUG
			delete g_pDeadLockProfiler;
#endif
			delete g_pMemoryAdminster;
		}

	private:
		MUTEX		m_Mutex;
	};
	CoreGlobal CoreGrobal;

} 
