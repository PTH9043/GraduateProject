#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USERVICE_H

#include "UObject.h"

BEGIN(Core)
class USession;
/*
@ Date: 2024-01-02, Writer: 박태현
@ Explain
- Player Session들을 관리하는 인터페이스로 Session들의 생성, 삭제, 전체 메시지 보내기 등을 관리한다. 
*/
class CACHE_ALGIN_CORE_DLL UService abstract : public UObject {
public:
	UService(OBJCON_CONSTRUCTOR, SERVICETYPE _Type);
	DESTRUCTOR(UService)
public:
	virtual _bool NativeConstruct() PURE;
	// 해당 함수가 
	virtual _bool Start() PURE;
public:
	virtual SHPTR<USession> FindSession(const SESSIONID _SessionID) PURE;
	// 전체 서버 참여자에게 메시지를 보내는 함수이다. 
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	virtual void LeaveService(const SESSIONID _SessionID) PURE;
	virtual void RemoveService(const SESSIONID _SessionID) PURE;
	virtual void InsertSession(SHPTR<USession> _spSession) PURE;
protected:
	virtual void Connect() PURE;
	virtual SESSIONID GiveID();
protected: /* Get Set */
	IOContext& GetIOContext(REF_RETURN) { return m_IOContext; }
	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	URWLock& GetRunTimeLock(REF_RETURN) { return m_RunTimeLock; }
	MUTEX& GetLastLock(REF_RETURN) { return m_LastLock; }
private:
	virtual void Free() override;
private:
	SERVICETYPE						m_ServiceType;
	ATOMIC<SESSIONID>			m_IDIssuance;
	ATOMIC<_llong>					m_CurrentSessionCount;
	// Boost/Asio Context
	IOContext								m_IOContext;
	TCPSOCKET							m_TcpSocket;
	// Lock
	URWLock								m_RunTimeLock;
	MUTEX									m_LastLock;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USERVICE_H