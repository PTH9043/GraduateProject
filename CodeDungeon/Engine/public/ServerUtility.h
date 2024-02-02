#pragma once

#include "UBase.h"

BEGIN(Engine)

using BUFFER = ARRAY < _char, MAX_BUFFER_LENGTH>;

enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND};

class UOverExp;

/*
@ Date: 2024-02-02, Writer: 박태현
@ Explain
- Server에 필요한 메소드들 모음
*/
class UServerMethods {
public:
	static _bool InitConnection(WSADATA* _pWsaData);
	static HANDLE CreateIocpHandle();
	static SOCKET CreateTcpSocket();
	static SOCKET CreateUdpSocket();
	static bool ServerToConnect(const SOCKET& _Socket, SOCKADDR_IN* _pSocketAddr);
public:
	static constexpr _int ADDR_SIZE{ sizeof(SOCKADDR_IN) + 16 };
};

/*
===========================================================
SocketUtils
===========================================================
UOverExp
===========================================================
*/
/*
@ Date: 2024-02-02, Writer: 박태현
@ Explain
- GetQueuedCompletionStatus를 통해서 받아올 OverExp
*/
class UOverExp {
public:
	UOverExp();
	UOverExp(_char* _Packet, const int _Size);
private:
	WSAOVERLAPPED		m_Over;
	WSABUF						m_wsaBuffer;
	BUFFER							m_Buffer;
	COMP_TYPE					m_CompType;
};

/*
===========================================================
UOverExp
===========================================================
UNetworkAddress
===========================================================
*/

/*
@ Date: 2024-02-02, Writer: 박태현
@ Explain
- Network를 어떻게 받아올지 정하는 클래스 
*/
class UNetworkAddress final : public UBase {
public:
	UNetworkAddress(SOCKADDR_IN _SocketAddr);
	UNetworkAddress(const _wstring& _strIPAddress, _uint _PortNumber);
	NO_COPY(UNetworkAddress)
	DESTRUCTOR(UNetworkAddress)
public:
	SOCKADDR_IN& GetSockAddr(REF_RETURN) { return m_SocketAddr; }
	const _uint GetPortNumber() const { return m_PortNumber; }
	const _wstring& GetIPAddress() const { return m_wstrIPAddress; }
private:
	IN_ADDR IpToAddress();
private:
	virtual void Free() override;

private:
	SOCKADDR_IN			m_SocketAddr;
	_uint							m_PortNumber;
	_wstring						m_wstrIPAddress;
};
/*
===========================================================
UNetworkAddress
===========================================================
*/

END

