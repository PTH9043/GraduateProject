#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREGROBAL_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREGROBAL_H

BEGIN(Core)
class UThreadManager;
/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain:  생성, 소멸자를 이용해서 서버가 종료될 때, 메모리가 제대로 해제되게 하기 위한 글로벌 클래스이다. 
*/
class CORE_DLL UCoreGrobal {
public:
	UCoreGrobal();
	NO_COPY(UCoreGrobal)
	~UCoreGrobal();
public: /* CoreGrobal */
	/*
	@ Explain: CoreGrobal의 내용을 초기화
	*/
	HRESULT ReadyCoreGrobal();
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();

private:
	MUTEX										m_Mutex;
	IOContext*									m_pContext;
	SHPTR<UThreadManager>	m_spThreadManager;
};

CORE_DLL extern		UCoreGrobal e_CoreGrobal;
END

#define GGROBAL Core::e_CoreGrobal

#endif 

