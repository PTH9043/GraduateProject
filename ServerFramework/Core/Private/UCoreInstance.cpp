#include "CoreDefines.h"
#include "UCoreInstance.h"
#include "UThreadManager.h"

namespace Core
{
	CORE_DLL SHPTR<UCoreInstance> e_CoreGrobal = Create<UCoreInstance>();
	UCoreInstance::UCoreInstance() : m_spThreadManager{ Create<Core::UThreadManager>() }
	{

	}

	bool UCoreInstance::NativeConstruct()
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
		m_spThreadManager->RegisterFunc(_CallBack, _Data);
	}

	void UCoreInstance::Join()
	{
		m_spThreadManager->Join();
	}

	void UCoreInstance::Free()
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		using namespace std;
		std::this_thread::sleep_for(100ms);

		m_spThreadManager.reset();
	}

	/*
	-----------------------------
	UThreadManager
	-----------------------------
	UDeadLockProfiler
	-----------------------------
	*/

}