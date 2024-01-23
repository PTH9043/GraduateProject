#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UROOM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UROOM_H

#include "UObject.h"

BEGIN(Core)

/*
@ Date: 2024-01-23, Writer: 박태현
@ Explain
- 일을 생성하는 클래스
*/
class UJobTimer abstract : public UObject {
public:
	UJobTimer(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(UJobTimer)
public:
	static void TimerThread(void* _pJobTimer);
	 void RunTimer();

	void TurnOffRunningThread() { m_isRunningThread = false; }
protected:
	virtual void TickTimer() PURE;
private:
	virtual void Free() override;
private:
	_bool	m_isRunningThread;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UROOM_H