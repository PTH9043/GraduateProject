#include "CoreDefines.h"
#include "UCoreInstance.h"
#include "UThreadManager.h"

namespace Core
{
	CORE_DLL SHPTR<UCoreInstance> e_CoreGrobal = Create<UCoreInstance>();
	UCoreInstance::UCoreInstance() : m_ThreadManager{ Create<Core::UThreadManager>() }, 
		m_Context{nullptr}
	{
	}

	UCoreInstance::~UCoreInstance()
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		if (nullptr != m_Context)
			m_Context->stop();
		using namespace std;
		std::this_thread::sleep_for(100ms);

		m_ThreadManager.reset();
	}

	HRESULT UCoreInstance::ReadyCoreGrobal()
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


	void UCoreInstance::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		m_ThreadManager->RegisterFunc(_CallBack, _Data);
	}

	void UCoreInstance::Join()
	{
		m_ThreadManager->Join();
	}

	/*
	-----------------------------
	UThreadManager
	-----------------------------
	UDeadLockProfiler
	-----------------------------
	*/

}