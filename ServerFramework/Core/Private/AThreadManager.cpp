#include "CoreDefines.h"
#include "AThreadManager.h"
#include "AJobTimer.h"

namespace Core
{
	AThreadManager::AThreadManager() : m_CurThreadNum{0}
	{
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void AThreadManager::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		RETURN_CHECK(m_CurThreadNum >= TLS::MAX_THREAD, ;);

		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		m_ThreadContainer.emplace_back(ThreadJoin, _CallBack, _Data, m_CurThreadNum++);
	}
	/*
	@ Date: 2024-01-23
	@ Writer: 박태현
	*/
	void AThreadManager::RegisterJobTimer(SHPTR<AJobTimer> _spJobTImer)
	{
		RETURN_CHECK(m_CurThreadNum >= TLS::MAX_THREAD, ;);

		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		m_ThreadContainer.emplace_back(ThreadJoin, AJobTimer::TimerThread, _spJobTImer.get(), m_CurThreadNum++);
		m_JobTimerContainer.push_back(_spJobTImer);
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void AThreadManager::Join()
	{
		for (auto& iter : m_ThreadContainer)
		{
			iter.join();
		}
	}
	void AThreadManager::Detach()
	{
		for (auto& iter : m_ThreadContainer)
		{
			iter.detach();
		}
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void AThreadManager::ThreadJoin(const THREADFUNC& _CallBack, void* _Data, _uint _CurrentThreadID)
	{
		InitTLS(_CurrentThreadID);
		_CallBack(_Data);
		DestroyTLS();
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void AThreadManager::InitTLS(_uint _CurrentThreadID)
	{
		TLS::g_ThreadID = _CurrentThreadID;
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	void AThreadManager::DestroyTLS()
	{

	}

	void AThreadManager::Free()
	{
		for (auto& iter : m_JobTimerContainer)
		{
			iter->TurnOffRunningThread();
		}
		m_JobTimerContainer.clear();
		m_ThreadContainer.clear();
	}
}