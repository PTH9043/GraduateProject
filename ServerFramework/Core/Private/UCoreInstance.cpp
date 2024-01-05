#include "CoreDefines.h"
#include "UCoreInstance.h"
#include "UThreadManager.h"
#include "URandomManager.h"

namespace Core
{
	UCoreInstance::UCoreInstance() : 
		m_spThreadManager{ Create<Core::UThreadManager>() }, 
		m_spRandomManager{Create<Core::URandomManager>()}
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


	/*
	-----------------------------
	UThreadManager
	-----------------------------
	URandomManager
	-----------------------------
	*/


	_int UCoreInstance::ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum)
	{
		return m_spRandomManager->ReturnRadomNumber(_iMinNum, _iMaxNum);
	}

	_int UCoreInstance::ReturnRadomNumber(const _int _iMaxNum)
	{
		return m_spRandomManager->ReturnRadomNumber(_iMaxNum);
	}

		/*
	-----------------------------
	UThreadManager
	-----------------------------
	UDeadLockProfiler
	-----------------------------
	*/


	void UCoreInstance::Free()
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		using namespace std;
		std::this_thread::sleep_for(100ms);

		m_spThreadManager.reset();
		m_spRandomManager.reset();
	}
}