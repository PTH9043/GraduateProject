#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASTRESSCLIENTSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASTRESSCLIENTSERVICE_H

#include "AService.h"

BEGIN(Core)
/*
@ Date: 2024-01-31, Writer: 박태현
@ Explain
-  StressClient에서 사용할 서비스를 정의함 
*/
class CORE_DLL AStressClientService abstract : public AService {
public: /* USING */
	using SESSIONCONTAINER = CONHASHMAP<SESSIONID, SHPTR<ASession>>;
public:
	AStressClientService(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(AStressClientService)
public:
	virtual _bool NativeConstruct() PURE;
	virtual _bool Start() override;
public:
	virtual SHPTR<ASession> FindSession(const SESSIONID _SessionID) override;
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
	virtual void LeaveService(const SESSIONID _SessionID) override;
	virtual void InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession) override;

	static void RunningThread(void* _pService);

	_bool IsRunningThread() const { return m_isRunningThread; }
	void StopThreadRunning() { m_isRunningThread.store(false); }
protected:
	virtual void Tick() PURE;
	virtual void Connect() PURE;
	virtual SESSIONID GiveID() override;
	// 연결이 끊어진 소켓들을 제거하여 메모리 확보
	void RemoveDisconnectSockets();
protected: /* get set */
	SESSIONCONTAINER& GetSessionContainer() { return m_SessionContainer; }
private:
	virtual void Free() PURE;
private:
	ATOMIC<_bool>							m_isRunningThread;
	// Session Conatiner
	SESSIONCONTAINER					m_SessionContainer;
	CONQUEUE<SESSIONID>			m_RemainIDQueue;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASTRESSCLIENTSERVICE_H