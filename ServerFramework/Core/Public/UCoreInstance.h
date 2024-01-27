#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

#include "UBase.h"

BEGIN(Core)
class UService;
class UThreadManager;
class URandomManager;
class ULogManager;
class USpaceManager;
class USession;
/*
@ Date: 2024-01-23
@ Writer: 박태현
@ Explain
- 생성, 소멸자를 이용해서 서버가 종료될 때, 메모리가 제대로 해제되게 하기 위한 클래스이다. 
무조건 한 개만 생성한다. 
*/
class CORE_DLL UCoreInstance : public UBase {
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	DESTRUCTOR(UCoreInstance)
public:
	void ReadyCoreInstance(SHPTR<UService> _spService);
	_bool Start();
public: /* Service */
	// ID를 통해서 SessionID
	 SHPTR<USession> FindSession(const SESSIONID _SessionID);
	// 전체 서버 참여자에게 메시지를 보내는 함수이다. 
	 void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	// Session의 TCP Socket 연결만 끊고 Insert나 기타 다른 함수들을 정의해서 Socket들 제거
	 void LeaveService(const SESSIONID _SessionID);
	// Session을 Container에 저장하는 함수이다. 
	 void InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession);

public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();
public: /* RandomManager */
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);
public: /* LogManager */
	void PrintOut(const char* _fmt, ...);
	void FileOut(const char* _fmt, ...);
public: /* SpaceManager */
	void BuildGameSpace(const SPACEINFO& _SpaceInfo);

private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<UService>						m_spService;
	SHPTR<UThreadManager>		m_spThreadManager;
	SHPTR<URandomManager>		m_spRandomManager;
	SHPTR<ULogManager>				m_spLogManager;
	SHPTR<USpaceManager>			m_spSpaceManager;
};

END


#endif 

