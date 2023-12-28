#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UDEADLOCKPROFILER
#define _SERVERFRAMEWORK_CORE_PUBLIC_UDEADLOCKPROFILER

#include "UBase.h"

BEGIN(Core)

enum class DEADLOCKLOG : _ushort{

};


class UDeadLockProfiler final : public UBase {
public:
	UDeadLockProfiler();
	NO_COPY(UDeadLockProfiler)
	DESTRUCTOR(UDeadLockProfiler)

public:
	void PushLock(DEADLOCKLOG _eDeadLockLog);
	void PopLock(DEADLOCKLOG _eDeadLockLog);
	void CheckCycle();
private:
	void Dfs(const _long _Index);

private:
	virtual void Free() override;

private:
	CONUNORMAP<DEADLOCKLOG, _long>		m_NameTold;
	CONUNORMAP<_long, DEADLOCKLOG>		m_idToName;

};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UDEADLOCKPROFILER