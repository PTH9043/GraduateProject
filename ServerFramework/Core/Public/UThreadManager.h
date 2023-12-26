#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_THREADMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_THREADMANAGER_H

#include "UBase.h"


BEGIN(Core)
template<class T>
using VECTOR = std::vector<T>;
using THREAD = std::thread;
using THRAEDVECTOR = VECTOR<THREAD>;

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain:  Thread를 관리하는 매니저 클래스 
*/
class CACHE_ALGIN_CORE_DLL UThreadManager final : public UBase {
public:
	UThreadManager();
	NO_COPY(UThreadManager)
	DESTRUCTOR(UThreadManager)

public:
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain:  실제 쓰레드를 등록하는 함수
	*/
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain:  실제 쓰레드를 실행하는 함수
	*/
	void Join();
private:
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain:  쓰레드에 등록하는 함수
	*/
	static void ThreadJoin(const THREADFUNC& _CallBack, void* _Data, _uint _iCurrentThreadID);
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	static void InitTLS(_uint _iCurrentThreadID);
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	*/
	static void DestroyTLS();

	// 초기화 함수 -> UBase에 기록됨 
	virtual void Free() override;
private:
	MUTEX							m_Mutex;
	THRAEDVECTOR			m_ThreadContainer;
	// Thread Num
	std::atomic< _uint>		m_iCurThreadNum;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_THREADMANAGER_H