#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UTHREADMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UTHREADMANAGER_H

#include "UBase.h"


BEGIN(Core)
using THREAD = std::thread;
using THRAEDVECTOR = VECTOR<THREAD>;

/*
@ Date: 2023-12-26, Writer: 박태현
@ Explain
- Thread를 관리하는 매니저 클래스 
*/
class CACHE_ALGIN UThreadManager final : public UBase {
public:
	UThreadManager();
	NO_COPY(UThreadManager)
	DESTRUCTOR(UThreadManager)

public:
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();
private:
	static void ThreadJoin(const THREADFUNC& _CallBack, void* _Data, _uint _CurrentThreadID);
	static void InitTLS(_uint _CurrentThreadID);
	static void DestroyTLS();

	// 초기화 함수 -> UBase에 기록됨 
	virtual void Free() override;
private:
	MUTEX							m_Mutex;
	THRAEDVECTOR			m_ThreadContainer;
	// Thread Num
	std::atomic< _uint>		m_CurThreadNum;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UTHREADMANAGER_H