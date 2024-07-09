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
	class CORE_DLL ARWLock {
	public:
		ARWLock();
		ARWLock(const ARWLock& _rhs);
		~ARWLock();

		ARWLock& operator =(const ARWLock& _lock);
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
		ReadLockGuard(const ARWLock& _Lock, const char* _name) : m_RunTimeLock{ _Lock }, m_Name{ _name } { m_RunTimeLock.ReadLock(_name); }
		~ReadLockGuard() { m_RunTimeLock.ReadUnLock(m_Name); }
#else
		ReadLockGuard(const RWLock& _Lock) : m_Lock{ _Lock }, { m_Lock.ReadLock(); }
		~ReadLockGuard() { m_Lock.ReadUnLock(); }
#endif

	private:
		ARWLock		   m_RunTimeLock;

#ifdef USE_DEBUG
		const char* m_Name;
#endif
	};

	class WriteLockGuard {
	public:
#ifdef USE_DEBUG
		WriteLockGuard(const ARWLock& _Lock, const char* _name) : m_RunTimeLock{ _Lock }, m_Name{ _name } { m_RunTimeLock.WriteLock(_name); }
		~WriteLockGuard() { m_RunTimeLock.WriteUnLock(m_Name); }
#else
		WriteLockGuard(const RWLock& _Lock) : m_Lock{ _Lock }, { m_Lock.WriteLock(); }
		~WriteLockGuard() { m_Lock.WriteUnLock(); }
#endif

	private:
		ARWLock			m_RunTimeLock;

#ifdef USE_DEBUG
		const char* m_Name;
#endif 
	};


	/*
	@ Date: 2024-01-06,  Writer: 박태현
	@ Explain
	- AWS 버전에서 따온 SpinLock
	*/
	class CORE_DLL AFastSpinLock {
	public:
		enum LockFlag
		{
			LF_WRITE_MASK = 0x7FF00000,
			LF_WRITE_FLAG = 0x00100000,
			LF_READ_MASK = 0x000FFFFF
		};
	public:
		AFastSpinLock();
		AFastSpinLock(const AFastSpinLock& _rhs);
		AFastSpinLock& operator =(const AFastSpinLock& _lock);
		~AFastSpinLock();

		/// exclusive mode
		void EnterWriteLock();
		void LeaveWriteLock();

		/// shared mode
		void EnterReadLock();
		void LeaveReadLock();

		long long  GetLockFlag() const { return m_LockFlag.load(std::memory_order_seq_cst); }
	private:
		ATOMIC<_llong>	m_LockFlag;
	};

	class AReadSpinLockGuard {
	public:
		AReadSpinLockGuard() : m_SpinLock{} {}
		AReadSpinLockGuard(const AFastSpinLock& _Lock) : m_SpinLock{ _Lock } { m_SpinLock.EnterReadLock(); }
		~AReadSpinLockGuard() { m_SpinLock.LeaveReadLock(); }

		AReadSpinLockGuard& operator()(const AFastSpinLock& _Lock)
		{
			m_SpinLock = _Lock;
			m_SpinLock.EnterReadLock();
			return *this;
		}

	private:
		AFastSpinLock			m_SpinLock;
	};

	class AWriteSpinLockGuard {
	public:
		AWriteSpinLockGuard() : m_SpinLock{}{}
		AWriteSpinLockGuard(const AFastSpinLock& _Lock) : m_SpinLock{ _Lock } { m_SpinLock.EnterWriteLock(); }
		~AWriteSpinLockGuard() { m_SpinLock.LeaveWriteLock(); }

		AWriteSpinLockGuard& operator()(const AFastSpinLock& _Lock)
		{
			m_SpinLock = _Lock;
			m_SpinLock.EnterWriteLock();
			return *this;
		}

	private:
		AFastSpinLock			m_SpinLock;
	};

	/*
	@ Date: 2024-01-06,  Writer: 박태현
	@ Explain
	- AWS 버전에서 따온 SpinLock
	*/
	class CORE_DLL UGuardSpinLock {
	public:

	};
}
#endif 