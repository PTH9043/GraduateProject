#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H

#include "ACoreObject.h"

BEGIN(Core)

/*
@ Date: 2024-01-23, Writer: 박태현
@ Explain
- 일을 생성하는 클래스로 해당 클래스를 통해 몬스터의 AI, 실시간으로 DB에 저장이 필요한 값들을 
저장할 수 있다. 
*/
class AJobTimer abstract : public ACoreObject {
public:
	AJobTimer(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(AJobTimer)
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

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H