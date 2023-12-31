#include "CoreDefines.h"
#include <vector>
#include "UThreadManager.h"

namespace Core
{
	UThreadManager::UThreadManager() : m_CurThreadNum{0}
	{
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void UThreadManager::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };

		m_ThreadContainer.emplace_back(ThreadJoin, _CallBack, _Data, m_CurThreadNum++);
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
	void UThreadManager::ThreadJoin(const THREADFUNC& _CallBack, void* _Data, _uint _CurrentThreadID)
	{
		InitTLS(_CurrentThreadID);
		_CallBack(_Data);
		DestroyTLS();
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void UThreadManager::InitTLS(_uint _CurrentThreadID)
	{
		TLS::g_ThreadID = _CurrentThreadID;
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