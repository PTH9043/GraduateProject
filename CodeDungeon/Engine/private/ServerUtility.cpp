#include "EngineDefine.h"
#include "ServerUtility.h"



_bool UServerMethods::InitConnection(WSADATA* _pWsaData)
{
	std::wcout.imbue(std::locale("Korean"));
	_int Error = WSAStartup(MAKEWORD(2, 0), _pWsaData);
	return 0 != Error;
}

HANDLE UServerMethods::CreateIocpHandle()
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}

SOCKET UServerMethods::CreateTcpSocket()
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

SOCKET UServerMethods::CreateUdpSocket()
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool UServerMethods::ServerToConnect(const SOCKET& _Socket, SOCKADDR_IN* _pSocketAddr)
{
	_int iResult = WSAConnect(_Socket, reinterpret_cast<sockaddr*>(_pSocketAddr),
		sizeof(SOCKADDR_IN), 0, 0, 0, 0);
	return 0 != iResult;
}

/*
===========================================================
USeverUtils
===========================================================
UNetworkAddress
===========================================================
*/

UOverExp::UOverExp()
{
	m_wsaBuffer.len = MAX_BUFFER_LENGTH;
	m_wsaBuffer.buf = &m_Buffer[0];
	m_CompType = OP_RECV;
	ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));
}

UOverExp::UOverExp(_char* _Packet, const int _Size)
{
	m_wsaBuffer.len = MAX_BUFFER_LENGTH;
	m_wsaBuffer.buf = &m_Buffer[0];
	m_CompType = OP_SEND;
	ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));
	memcpy(&m_Buffer[0], _Packet, _Size);
}
/*
===========================================================
UOverExp
===========================================================
UNetworkAddress
===========================================================
*/
UNetworkAddress::UNetworkAddress(SOCKADDR_IN _SocketAddr) : m_SocketAddr{_SocketAddr}
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &m_SocketAddr.sin_addr, buffer, sizeof(buffer));
	m_wstrIPAddress = buffer;
	m_PortNumber = ::ntohs(m_SocketAddr.sin_port);
}

UNetworkAddress::UNetworkAddress(const _wstring& _wstrIPAddress, _uint _PortNumber) : 
	m_wstrIPAddress{_wstrIPAddress}, m_PortNumber{_PortNumber}
{
	::memset(&m_SocketAddr, 0, sizeof(SOCKADDR_IN));
	m_SocketAddr.sin_family = AF_INET;
	m_SocketAddr.sin_addr = IpToAddress();
	m_SocketAddr.sin_port = ::htons(m_PortNumber);
}

IN_ADDR UNetworkAddress::IpToAddress()
{
	IN_ADDR address;
	::InetPtonW(AF_INET, m_wstrIPAddress.c_str(), &address);
	return address;
}

void UNetworkAddress::Free()
{
}