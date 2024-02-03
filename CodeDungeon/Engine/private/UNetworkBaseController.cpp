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
		
	}
}

void UNetworkBaseController::ProcessPacket(_char* _pPacket, _llong _Size)
{
}

void UNetworkBaseController::RecvTcpPacket()
{
	UServerMethods::RecvTcpPacket(m_ClientTcpSocket, REF_IN m_RecvTcpOverExp);
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
