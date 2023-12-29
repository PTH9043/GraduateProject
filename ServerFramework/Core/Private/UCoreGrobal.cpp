#include "CoreDefines.h"
#include "UCoreGrobal.h"
#include "UThreadManager.h"
#include "UDeadLockProfiler.h"

namespace Core
{
	CORE_DLL UCoreGrobal e_CoreGrobal;

	UCoreGrobal::UCoreGrobal() : m_spThreadManager{ Create<Core::UThreadManager>() }, 
		m_spDeadLockProfiler{ Create<UDeadLockProfiler>() },
		m_pContext{nullptr}
	{
	}

	UCoreGrobal::~UCoreGrobal()
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		if (nullptr != m_pContext)
			m_pContext->stop();
		using namespace std;
		std::this_thread::sleep_for(100ms);

		m_spThreadManager.reset();
	}

	HRESULT UCoreGrobal::ReadyCoreGrobal()
	{

		return S_OK;
	}

	/*
	-----------------------------
	CoreGrobal
	-----------------------------
	UThreadManager
	-----------------------------
	*/


	void UCoreGrobal::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		m_spThreadManager->RegisterFunc(_CallBack, _Data);
	}

	void UCoreGrobal::Join()
	{
		m_spThreadManager->Join();
	}

	/*
	-----------------------------
	UThreadManager
	-----------------------------
	UDeadLockProfiler
	-----------------------------
	*/

	void UCoreGrobal::PushLock(const char* _DeadLockLog)
	{
		m_spDeadLockProfiler->PushLock(_DeadLockLog);
	}

	void UCoreGrobal::PopLock(const char* _DeadLockLog)
	{
		m_spDeadLockProfiler->PopLock(_DeadLockLog);
	}

	void UCoreGrobal::CheckCycle()
	{
		m_spDeadLockProfiler->CheckCycle();
	}

	/*
	-----------------------------
	UDeadLockProfiler
	-----------------------------
	UDeadLockProfiler
	-----------------------------
	*/

}