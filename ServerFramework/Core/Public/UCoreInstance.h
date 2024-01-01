#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

#include "UBase.h"

BEGIN(Core)
class UThreadManager;
/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain
- 생성, 소멸자를 이용해서 서버가 종료될 때, 메모리가 제대로 해제되게 하기 위한 클래스이다. 
무조건 한 개만 생성한다. 
*/
class CACHE_ALGIN_CORE_DLL UCoreInstance : public UBase {
	DECLARE_ONCEDATA
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	DESTRUCTOR(UCoreInstance)

public: /* CoreGrobal */
	bool NativeConstruct();
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();

public:

private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<UThreadManager>		m_ThreadManager;
};

END


#endif 

