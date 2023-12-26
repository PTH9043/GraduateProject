#include "CoreDefines.h"
#include "UCoreGrobal.h"
#include "UThreadManager.h"


namespace Core
{
	CORE_DLL UCoreGrobal e_CoreGrobal;

	UCoreGrobal::UCoreGrobal() : m_spThreadManager{ Create<Core::UThreadManager>() }, 
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

	void UCoreGrobal::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		m_spThreadManager->RegisterFunc(_CallBack, _Data);
	}

	void UCoreGrobal::Join()
	{
		m_spThreadManager->Join();
	}

}