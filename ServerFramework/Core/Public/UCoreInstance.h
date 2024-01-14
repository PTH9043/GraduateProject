#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

#include "UBase.h"

BEGIN(Core)
class UThreadManager;
class URandomManager;
class ULogManager;
/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain
- 생성, 소멸자를 이용해서 서버가 종료될 때, 메모리가 제대로 해제되게 하기 위한 클래스이다. 
무조건 한 개만 생성한다. 
*/
class CACHE_ALGIN_CORE_DLL UCoreInstance : public UBase {
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	DESTRUCTOR(UCoreInstance)
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();
public: /* RandomManager */
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);

public: /* LogManager */
	void PrintOut(const char* _fmt, ...);
	void FileOut(const char* _fmt, ...);

private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<UThreadManager>		m_spThreadManager;
	SHPTR<URandomManager>		m_spRandomManager;
	SHPTR<ULogManager>				m_spLogManager;
};

END


#endif 

