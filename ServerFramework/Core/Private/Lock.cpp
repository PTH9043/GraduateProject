#include "CoreDefines.h"
#include "Lock.h"
#include "UDeadLockProfiler.h"

namespace Core
{
	/*
	@ Date: 2023-12-27
	@ Writer: 박태현
	@	 Explain
	- 아무도 공유하지 않을 경우 Write Lock을 걸어서 자신이 원하는 값이 실행되기를 바란다.
	*/
	URWLock::URWLock() : m_lockFlag{ 0 }, m_WriteCount{ 0 } {
	}
	URWLock::URWLock(const URWLock& _rhs)
	{
		m_lockFlag.store(_rhs.m_lockFlag.load());
		m_WriteCount.store(_rhs.m_WriteCount.load());
	}
	URWLock::~URWLock()
	{
	}
	URWLock& URWLock::operator=(const URWLock& _lock)
	{
		m_lockFlag.store(_lock.m_lockFlag.load());
		m_WriteCount.store(_lock.m_WriteCount.load());
		return *this;
	}

#ifdef USE_DEBUG
	void URWLock::WriteLock(const char* _name)
#else
	void RWLock::WriteLock()
#endif
	{

#ifdef USE_DEBUG
		g_pDeadLockProfiler->PushLock(_name);
#endif

		// 상위 16비트만 추출한다. 
		const _llong LockThreadID = (m_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

		// 만약 동일한 쓰레드가 Lock을 소유하고 있으면 무조건 성공시킨다. 
		if (LockThreadID == TLS::g_ThreadID)
		{
			++m_WriteCount;
			return;
		}
		// 10초
		auto BeginTime = std::chrono::high_resolution_clock::now();

		// 소유권을 가져온다. 
		const _llong Desired = ((static_cast<_llong>(TLS::g_ThreadID) << 16) & WRITE_THREAD_MASK);
		while (true)
		{
			for (_uint SpinCount = 0; SpinCount < MAX_SPIN_COUNT; ++SpinCount)
			{
				_llong Expected = EMPTY_FLAG;
				// Lock Flag가 EMPTY FLAG와 같다면 LockFlag의 값 Desired로 변경
				if (m_lockFlag.compare_exchange_strong(REF_OUT Expected, Desired))
				{
					// Write Count 증가
					++m_WriteCount;
					return;
				}
			}
			// 10초 이상일 경우 해당 반복문을 빠져나온다. 
			auto EndTime = BeginTime - std::chrono::high_resolution_clock::now();
			if (duration_cast<std::chrono::milliseconds>(EndTime).count() >= ACOUIRE_TIMEOUT_TICK)
			{
				CRASH("LOCK_TIMEOUT");
			}

			// 일단 해당 쓰레드는 잠시 쉬게 둔다. 
			std::this_thread::yield();
		}
	}
	/*
	@ Date: 2023-12-27
	@ Writer: 박태현
	*/
#ifdef USE_DEBUG
	void URWLock::WriteUnLock(const char* _name)
#else
	void RWLock::WriteUnLock()
#endif
	{
#ifdef USE_DEBUG
		g_pDeadLockProfiler->PopLock(_name);
#endif

		// ReadLock을 다 풀기전에 WriteLock은 불가능
		if ((m_lockFlag.load() & READ_THREAD_MASK) != 0)
		{
			CRASH("INVALID_UNLOCK_ORDER");
		}

		const _uint WRITECOUNT = --m_WriteCount;
		if (0 == WRITECOUNT)
		{
			m_lockFlag.store(EMPTY_FLAG);
		}
	}
	/*
	@ Date: 2023-12-27
	@ Writer: 박태현
	*/
#ifdef USE_DEBUG
	void URWLock::ReadLock(const char* _name)
#else 
	void RWLock::ReadLock()
#endif
	{
#ifdef USE_DEBUG
		g_pDeadLockProfiler->PushLock(_name);
#endif

		// 상위 16비트만 추출한다. 
		const _llong LockThreadID = (m_lockFlag.load() & READ_THREAD_MASK) >> 16;
		// 만약 동일한 쓰레드가 Lock을 소유하고 있으면 무조건 성공시킨다. 
		if (LockThreadID == TLS::g_ThreadID)
		{
			m_lockFlag.fetch_add(1);
			return;
		}
		auto BeginTime = std::chrono::high_resolution_clock::now();
		// 소유권을 가져온다. 
		while (true)
		{
			for (_uint SpinCount = 0; SpinCount < MAX_SPIN_COUNT; ++SpinCount)
			{
				_llong Expected = (m_lockFlag.load() & READ_THREAD_MASK);
				// Lock Flag가 EMPTY FLAG와 같다면 LockFlag의 값 Desired로 변경
				if (m_lockFlag.compare_exchange_strong(REF_OUT Expected, Expected + 1))
				{
					return;
				}
			}
			// 10초 이상일 경우 해당 반복문을 빠져나온다. 
			auto EndTime = BeginTime - std::chrono::high_resolution_clock::now();
			if (duration_cast<std::chrono::milliseconds>(EndTime).count() >= ACOUIRE_TIMEOUT_TICK)
			{
				CRASH("LOCK_TIMEOUT");
			}

			// 일단 해당 쓰레드는 잠시 쉬게 둔다. 
			std::this_thread::yield();
		}
	}
	/*
	@ Date: 2023-12-27
	@ Writer: 박태현
	*/
#ifdef USE_DEBUG
	void URWLock::ReadUnLock(const char* _name)
#else
	void RWLock::ReadUnLock()
#endif
	{
#ifdef USE_DEBUG
		g_pDeadLockProfiler->PopLock(_name);
#endif

		if ((m_lockFlag.fetch_sub(1) & READ_THREAD_MASK) == 0)
		{
			CRASH("MUTIPLE_UNLOCK");
		}
	}

	/*
	============================================
	RWLock 
	============================================
	FastSpinLock
	============================================
	*/

	UFastSpinLock::UFastSpinLock() : m_LockFlag{ 0 } { }

	UFastSpinLock::UFastSpinLock(const UFastSpinLock& _rhs) :
		m_LockFlag{ _rhs.m_LockFlag.load() } {}

	UFastSpinLock& UFastSpinLock::operator=(const UFastSpinLock& _lock)
	{
		m_LockFlag.store(_lock.m_LockFlag, std::memory_order_seq_cst);
		return *this;
	}

	UFastSpinLock::~UFastSpinLock(){ }

	void UFastSpinLock::EnterWriteLock()
	{
		while (true)
		{
			while (m_LockFlag.load(std::memory_order_relaxed) & LF_WRITE_MASK)
				std::this_thread::yield();

			
			if ((m_LockFlag.fetch_add(LF_WRITE_FLAG, std::memory_order_acquire) & LF_WRITE_MASK) == LF_WRITE_FLAG)
			{
				while (m_LockFlag.load(std::memory_order_relaxed) & LF_READ_MASK)
					std::this_thread::yield();
				return;
			}
			m_LockFlag.fetch_sub(LF_WRITE_FLAG, std::memory_order_relaxed);
		}
	}

	void UFastSpinLock::LeaveWriteLock()
	{
		m_LockFlag.fetch_sub(LF_WRITE_FLAG, std::memory_order_relaxed);
	}

	void UFastSpinLock::EnterReadLock()
	{
		while (true)
		{
			while (m_LockFlag.load(std::memory_order_relaxed) & LF_WRITE_MASK)
				std::this_thread::yield();

			if ((m_LockFlag.fetch_add(1, std::memory_order_acquire) & LF_WRITE_MASK) == LF_WRITE_FLAG)
			{
				return;
			}
			m_LockFlag.fetch_sub(1, std::memory_order_relaxed);
		}
	}

	void UFastSpinLock::LeaveReadLock()
	{
		m_LockFlag.fetch_sub(1, std::memory_order_relaxed);
	}
}