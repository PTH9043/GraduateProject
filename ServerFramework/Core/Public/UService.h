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
	DECLARE_ONCEDATA
public:
	using TCPACCEPTOR = Asio::ip::tcp::acceptor;
	using TCPSOCKET = Asio::ip::tcp::socket;
	using UDPSOCKET = Asio::ip::udp::socket;
public:
	UService(OBJCON_CONSTRUCTOR, SERVICETYPE _Type);
	DESTRUCTOR(UService)
public:
	virtual _bool NativeConstruct() PURE;
	// 해당 함수가 
	virtual _bool Start() PURE;
public: /* Get Set */
	const _llong GetCurSessionCount() { return m_CurrentSessionCount.load(std::memory_order_seq_cst); }
public:
	virtual SHPTR<USession> FindSession(const SESSIONID _SessionID) PURE;
	// 전체 서버 참여자에게 메시지를 보내는 함수이다. 
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	virtual void LeaveService(const SESSIONID _SessionID) PURE;
	virtual void RemoveService(const SESSIONID _SessionID) PURE;
protected:
	virtual void InsertSession(SHPTR<USession> _spSession) PURE;
	// Accept 함수를 실행시킨다. 
	virtual void AsyncAccept() PURE;
	virtual SESSIONID GiveID();
protected: /* Get Set */
	IOContext& GetIOContext(REF_RETURN) { return m_IOContext; }
	TCPACCEPTOR& GetTcpAccepctor(REF_RETURN){ return m_TcpAcceptor; }
	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	UDPSOCKET& GetUdpSocket(REF_RETURN) { return m_UdpSocket; }
	URWLock& GetRunTimeLock(REF_RETURN) { return m_RunTimeLock; }
	MUTEX& GetLastLock(REF_RETURN) { return m_LastLock; }
private:
	virtual void Free() override;
private:
	SERVICETYPE						m_ServiceType;
	SESSIONID								m_IDIssuance;
	ATOMIC<_llong>					m_CurrentSessionCount;
	// Boost/Asio Context
	IOContext								m_IOContext;
	// Tcps
	TCPACCEPTOR						m_TcpAcceptor;
	TCPSOCKET							m_TcpSocket;
	// UdpSocket
	UDPSOCKET							m_UdpSocket;
	// Lock
	URWLock								m_RunTimeLock;
	MUTEX									m_LastLock;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USERVICE_H