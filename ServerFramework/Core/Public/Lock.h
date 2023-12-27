#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_LOCK_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_LOCK_H

BEGIN(Core)


BEGIN(Lock)

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
@ Explain:  RW Lock 정의

W -> R (O)
R -> W (X)
*/
class  RWLock {
public:
	RWLock();
	RWLock(const RWLock& _rhs);
	~RWLock();

	RWLock& operator =(const RWLock& _lock);
	/*
	@ Explain:  WriteLock 해당하는 Lock이 걸릴 동안, 모든 Lock은 동작하지 않는다. 
	*/
	void WriteLock();
	/*
	@ Explain:  WriteLock 해제
	*/
	void WriteUnLock();
	/*
	@ Explain:  ReadLock 같은 Read Lock이 걸려있어도 그냥 통과하지만 WriteLock이 걸릴 경우 대기
	*/
	void ReadLock();
	/*
	@ Explain:  ReadLock 해제 
	*/
	void ReadUnLock();
private:
	ATOMIC<_llong>	m_lockFlag;
	ATOMIC<_uint>	m_WriteCount;
};

class ReadLockGuard {
public:
	ReadLockGuard(const RWLock& _Lock) : m_Lock{ _Lock } { m_Lock.ReadLock(); }
	~ReadLockGuard() { m_Lock.ReadUnLock(); }

private:
	RWLock		m_Lock;
};

class WriteLockGuard {
public:
	WriteLockGuard(const RWLock& _Lock) : m_Lock{ _Lock } { m_Lock.WriteLock(); }
	~WriteLockGuard() { m_Lock.WriteUnLock(); }

private:
	RWLock		m_Lock;
};

END

END

#endif 
