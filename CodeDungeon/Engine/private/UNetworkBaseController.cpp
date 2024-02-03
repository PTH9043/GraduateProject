#include "EngineDefine.h"
#include "UNetworkBaseController.h"
#include "UGameInstance.h"

UNetworkBaseController::UNetworkBaseController() : 
	m_isNetworkTickRunning{true},
	m_IocpHandle{NULL},
	m_RecvTcpOverExp{},
	m_ClientTcpSocket{NULL}, 
	m_ClientUdpSocket{NULL},
	m_WsaData{},
	m_TcpTotalBuffer{},
	m_RemainBufferLength{0},
	m_spNetworkAddress{nullptr}
{
}

HRESULT UNetworkBaseController::NativeConstruct(const _wstring& _wstrIPAddress, const _int _PortNumber)
{
	// Create Socket
	m_IocpHandle = UServerMethods::CreateIocpHandle();
	m_ClientTcpSocket = UServerMethods::CreateTcpSocket(WSA_FLAG_OVERLAPPED);
	UServerMethods::RegisterIocpToSocket(m_ClientTcpSocket, m_IocpHandle);
	m_spNetworkAddress = Create<UNetworkAddress>(_wstrIPAddress, _PortNumber);

	// Wsa Connect 
	RETURN_CHECK(UServerMethods::ReadyConnectToServer(&m_WsaData), E_FAIL);	
	RETURN_CHECK(UServerMethods::ServerToConnect(m_ClientTcpSocket, m_spNetworkAddress), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	spGameInstance->RegisterFuncToRegister(ServerThread, this);

	RecvTcpPacket();
	return S_OK;
}

void UNetworkBaseController::ServerTick()
{
	DWORD num_bytes;
	ULONG_PTR key;
	WSAOVERLAPPED* over = nullptr;
	BOOL ret = GetQueuedCompletionStatus(m_IocpHandle, &num_bytes, &key, &over, INFINITE);
	UOverExp* ex_over = reinterpret_cast<UOverExp*>(over);
	
	if (FALSE == ret) {
		ASSERT_CRASH("Failed Connect");
		if (ex_over->GetCompType() < OP_SEND_END)
			Make::xdelete(ex_over);
	}

	if ((0 == num_bytes) && ((ex_over->GetCompType() == OP_TCP_RECV) || (ex_over->GetCompType() == OP_TCP_SEND))) {
		ASSERT_CRASH("Failed Connect");
		if (ex_over->GetCompType() < OP_SEND_END)
			Make::xdelete(ex_over);
	}

	switch (ex_over->GetCompType()) {
	case OP_TCP_SEND:
	case OP_UDP_SEND:
		Make::xdelete(ex_over);
		break;
	case OP_TCP_RECV:
		TcpPacketRecv(ex_over, num_bytes);
		RecvTcpPacket();
		break;
	}
}

void UNetworkBaseController::TcpPacketRecv(UOverExp* _pOverExp, _llong _numBytes)
{
	// Total Buffer에 값을 복사 
	::memcpy(&m_TcpTotalBuffer[m_RemainBufferLength], _pOverExp->GetBufferAddress(), _numBytes);
	_llong value = _numBytes + m_RemainBufferLength;
	char* pBuffer = &m_TcpTotalBuffer[0];
	_llong MoveBufferValue = 0;
	while (value > 0)
	{
		PACKETHEAD Head;
		::memcpy(&Head, &m_TcpTotalBuffer[0], PACKETHEAD_SIZE);
		short CurrentPacketSize = Head.PacketSize + PACKETHEAD_SIZE;
		if ((m_RemainBufferLength - CurrentPacketSize) < 0)
		{
			::memmove(&m_TcpTotalBuffer[0], pBuffer, MAX_PROCESSBUF_LENGTH - MoveBufferValue);
			return;
		}
		ProcessPacket(&pBuffer[MoveBufferValue], Head);
		MoveBufferValue -= CurrentPacketSize;
		pBuffer += CurrentPacketSize;
		MoveBufferValue += CurrentPacketSize;
	}
	m_RemainBufferLength = 0;
	ZeroMemory(&m_TcpTotalBuffer[0], MAX_PROCESSBUF_LENGTH);
}

void UNetworkBaseController::RecvTcpPacket()
{
	UServerMethods::RecvTcpPacket(m_ClientTcpSocket, REF_OUT m_RecvTcpOverExp);
}

void UNetworkBaseController::SendTcpPacket(_char* _pPacket, _short _PacketType, _short _PacketSize)
{
	UOverExp* pOverExp = Make::xnew<UOverExp>(_pPacket, _PacketType, _PacketSize);
	UServerMethods::SendTcpPacket(m_ClientTcpSocket, pOverExp);
}

void UNetworkBaseController::ServerThread(void* _pData)
{
	UNetworkBaseController* pNetworkBaseController = static_cast<UNetworkBaseController*>(_pData);
	RETURN_CHECK(nullptr == pNetworkBaseController, ;);
	while (pNetworkBaseController->m_isNetworkTickRunning)
	{
		pNetworkBaseController->ServerTick();
	}
}

void UNetworkBaseController::Free()
{
	m_isNetworkTickRunning = false;
	closesocket(m_ClientTcpSocket);
	WSACleanup();
}
