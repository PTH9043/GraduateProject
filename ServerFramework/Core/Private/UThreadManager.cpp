#include "CoreDefines.h"
#include "UThreadManager.h"
#include "UJobTimer.h"

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
		RETURN_CHECK(m_CurThreadNum >= TLS::MAX_THREAD, ;);

		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		m_ThreadContainer.emplace_back(ThreadJoin, _CallBack, _Data, m_CurThreadNum++);
	}
	/*
	@ Date: 2024-01-23
	@ Writer: 박태현
	*/
	void UThreadManager::RegisterJobTimer(SHPTR<UJobTimer> _spJobTImer)
	{
		RETURN_CHECK(m_CurThreadNum >= TLS::MAX_THREAD, ;);

		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		m_ThreadContainer.emplace_back(ThreadJoin, UJobTimer::TimerThread, _spJobTImer.get(), m_CurThreadNum++);
		m_JobTimerContainer.push_back(_spJobTImer);
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
		m_JobTimerContainer.clear();
		m_ThreadContainer.clear();
	}
}