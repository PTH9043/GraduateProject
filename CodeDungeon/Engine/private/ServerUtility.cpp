#include "EngineDefine.h"
#include "ServerUtility.h"



_bool UServerMethods::ReadyConnectToServer(WSADATA* _pWsaData)
{
	std::wcout.imbue(std::locale("Korean"));
	_int Error = WSAStartup(MAKEWORD(2, 0), _pWsaData);
	return 0 != Error;
}

HANDLE UServerMethods::CreateIocpHandle()
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}

void UServerMethods::RegisterIocpToSocket(const SOCKET& _Socket, const HANDLE& _IocpHandle)
{
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(_Socket), _IocpHandle, 9999, 0);
}

SOCKET UServerMethods::CreateTcpSocket(_int _OverlappedValue)
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, _OverlappedValue);
}

SOCKET UServerMethods::CreateUdpSocket(_int _OverlappedValue)
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_UDP, NULL, 0, _OverlappedValue);
}

void UServerMethods::RecvTcpPacket(const SOCKET& _Socket, REF_IN UOverExp& _OverExp)
{
	DWORD recv_flag = 0;
	_OverExp.RecvReset();
	WSARecv(_Socket, _OverExp.GetWsaBuffPointer(), 1, 0, &recv_flag,	_OverExp.GetOverlappedPointer(), 0);
}

void UServerMethods::SendTcpPacket(const SOCKET& _Socket, UOverExp* _pOverExp)
{
	WSASend(_Socket, _pOverExp->GetWsaBuffPointer(), 1, 0, 0, _pOverExp->GetOverlappedPointer(), 0);
}

void UServerMethods::StartNonBlocking(const SOCKET& _Socket)
{
	// NonBlocking
	unsigned long noblock = 1;
	int nRet = ioctlsocket(_Socket, FIONBIO, &noblock);
}

bool UServerMethods::ServerToConnect(const SOCKET& _Socket, SOCKADDR_IN* _pSocketAddr)
{
	_int iResult = WSAConnect(_Socket, reinterpret_cast<sockaddr*>(_pSocketAddr),
		sizeof(SOCKADDR_IN), 0, 0, 0, 0);
	return 0 != iResult;
}

bool UServerMethods::ServerToConnect(const SOCKET& _Socket, CSHPTRREF<UNetworkAddress> _spNetworkAddress)
{
	_int iResult = WSAConnect(_Socket, reinterpret_cast<sockaddr*>(_spNetworkAddress->GetSockAddrPointer()),
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

UOverExp::UOverExp(COMP_TYPE _type)
{
	m_wsaBuffer.len = MAX_BUFFER_LENGTH;
	m_wsaBuffer.buf = &m_Buffer[0];
	m_CompType = _type;
	ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));
}

UOverExp::UOverExp(_char* _Packet, const int _Size, COMP_TYPE _type)
{
	m_wsaBuffer.len = _Size;
	m_wsaBuffer.buf = &m_Buffer[0];
	m_CompType = _type;
	ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));
	memcpy(&m_Buffer[0], _Packet, _Size);
}

UOverExp::UOverExp(_char* _pPacket, _short _PacketType, _short _PacketSize, COMP_TYPE _type)
{
	m_wsaBuffer.len = _PacketSize + PACKETSIZE_SIZE;
	m_wsaBuffer.buf = &m_Buffer[0];
	m_CompType = _type;
	ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));

	PACKETHEAD Packet{ _PacketSize, _PacketType };
	::memcpy(&m_Buffer[0], &Packet, PACKETHEAD_SIZE);
	::memcpy(&m_Buffer[PACKETHEAD_SIZE], _pPacket, _PacketSize);
}

void UOverExp::RecvReset(COMP_TYPE _type)
{
	m_wsaBuffer.len = MAX_BUFFER_LENGTH;
	m_wsaBuffer.buf = &m_Buffer[0];
	m_CompType = _type;
	ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));
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