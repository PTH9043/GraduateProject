#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

BEGIN(Core)
class UThreadManager;
/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain
- 생성, 소멸자를 이용해서 서버가 종료될 때, 메모리가 제대로 해제되게 하기 위한 글로벌 클래스이다. 
*/
class CACHE_ALGIN_CORE_DLL UCoreInstance {
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	~UCoreInstance();
public: /* CoreGrobal */
	HRESULT ReadyCoreGrobal();
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();

private:
	MUTEX											m_Mutex;
	IOContext*										m_Context;
	SHPTR<UThreadManager>		m_ThreadManager;
};

END


#endif 

