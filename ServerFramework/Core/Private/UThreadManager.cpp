#include "CoreDefines.h"
#include <vector>
#include "UThreadManager.h"

namespace Core
{
	UThreadManager::UThreadManager() : m_iCurThreadNum{0}
	{
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void UThreadManager::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };

		m_ThreadContainer.emplace_back(ThreadJoin, _CallBack, _Data, m_iCurThreadNum++);
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void UThreadManager::Join()
	{
		for (auto& iter : m_ThreadContainer)
		{
			iter.join();
		}
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void UThreadManager::ThreadJoin(const THREADFUNC& _CallBack, void* _Data, _uint _iCurrentThreadID)
	{
		InitTLS(_iCurrentThreadID);
		_CallBack(_Data);
		DestroyTLS();
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void UThreadManager::InitTLS(_uint _iCurrentThreadID)
	{
		TLS::g_ThreadID = _iCurrentThreadID;
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void UThreadManager::DestroyTLS()
	{

	}

	void UThreadManager::Free()
	{
	}
}