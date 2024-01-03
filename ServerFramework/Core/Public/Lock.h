#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_LOCK_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_LOCK_H

namespace Core {

	class UDeadLockProfiler;

	enum : _llong
	{
		ACOUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_THREAD_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

	/*
	=============================
				RW Spin Lock
	=============================
	*/
	/*
	[\\\\\\\\][\\\\\\\\][RRRRRRRR][RRRRRRRR]
	W: WriteFlag (Exclusive Lock Owner Thread)
	R: ReadFlag(Shared Lock Count)
	*/
	/*
	@ Date: 2023-12-27
	@ Writer: 박태현
	@ Explain
	- RW Lock 정의

	W -> R (O)
	R -> W (X)
	*/
	class CACHE_ALGIN_CORE_DLL URWLock {
	public:
		URWLock();
		URWLock(const URWLock& _rhs);
		~URWLock();

		URWLock& operator =(const URWLock& _lock);
#ifdef USE_DEBUG

		void WriteLock(const char* _name);
		void WriteUnLock(const char* _name);
		void ReadLock(const char* _name);
		void ReadUnLock(const char* _name);
#else 
		void WriteLock();
		void WriteUnLock();
		void ReadLock();
		void ReadUnLock();
#endif 
	private:
		ATOMIC<_llong>	m_lockFlag;
		ATOMIC<_uint>	m_WriteCount;
	};

	class ReadLockGuard {
	public:

#ifdef USE_DEBUG
		ReadLockGuard(const URWLock& _Lock, const char* _name) : m_RunTimeLock{ _Lock }, m_Name{ _name } { m_RunTimeLock.ReadLock(_name); }
		~ReadLockGuard() { m_RunTimeLock.ReadUnLock(m_Name); }
#else
		ReadLockGuard(const RWLock& _Lock) : m_Lock{ _Lock }, { m_Lock.ReadLock(); }
		~ReadLockGuard() { m_Lock.ReadUnLock(); }
#endif

	private:
		URWLock		   m_RunTimeLock;

#ifdef USE_DEBUG
		const char* m_Name;
#endif
	};

	class WriteLockGuard {
	public:
#ifdef USE_DEBUG
		WriteLockGuard(const URWLock& _Lock, const char* _name) : m_RunTimeLock{ _Lock }, m_Name{ _name } { m_RunTimeLock.WriteLock(_name); }
		~WriteLockGuard() { m_RunTimeLock.WriteUnLock(m_Name); }
#else
		WriteLockGuard(const RWLock& _Lock) : m_Lock{ _Lock }, { m_Lock.WriteLock(); }
		~WriteLockGuard() { m_Lock.WriteUnLock(); }
#endif

	private:
		URWLock			m_RunTimeLock;

#ifdef USE_DEBUG
		const char* m_Name;
#endif 
	};
}
#endif 
