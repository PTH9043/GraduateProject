#include "EngineDefine.h"
#include "UNetworkBaseController.h"
#include "UGameInstance.h"
#include "UActor.h"
#include "UProcessedData.h"
#include "UMethod.h"

UNetworkBaseController::UNetworkBaseController() : 
	m_iSceneID{0},
	m_isNetworkTickRunning{ false },
	m_RecvTcpOverExp{},
	m_ClientTcpSocket{NULL},
	m_ClientUdpSocket{NULL},
	m_WsaData{},
	m_TcpTotalBuffer{},
	m_CurrentBufferLength{0},
	m_RemainBufferLength{0},
	m_llNetworkOwnerID{0},
	m_spNetworkAddress{nullptr},
	m_NetworkActorContainer{},
	m_NetworkInitDataContainer{},
	m_pSendOverExp{nullptr}
{
	m_pSendOverExp = new UOverExp();
}

HRESULT UNetworkBaseController::NativeConstruct(const _string& _strIPAddress, const _int _PortNumber)
{
	// Create Socket
	m_spNetworkAddress = Create<UNetworkAddress>(_strIPAddress, _PortNumber);
	// Wsa Connect 
	RETURN_CHECK(UServerMethods::ReadyConnectToServer(&m_WsaData), E_FAIL);	
	m_ClientTcpSocket = UServerMethods::CreateTcpSocket();
	RETURN_CHECK(UServerMethods::ServerToConnect(m_ClientTcpSocket, m_spNetworkAddress), E_FAIL);

	u_long mode = 1;  // 1 to enable non-blocking socket
	if (ioctlsocket(m_ClientTcpSocket, FIONBIO, &mode) != NO_ERROR) {
		printf("ioctlsocket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return E_FAIL;
	}


	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	spGameInstance->RegisterFuncToRegister(ServerThread, this);
	return S_OK;
}

void UNetworkBaseController::MakeActorsInit(const VECTOR<SHPTR<UBase>>& _actorContainer)
{
	m_NetworkInitDataContainer.clear();
}

void UNetworkBaseController::MakeActorsTick()
{
	if (!m_NetworkInitDataContainer.empty())
		m_NetworkInitDataContainer.clear();
}

void UNetworkBaseController::AddNetworkInitData(_int _NetworkID, const NETWORKRECEIVEINITDATA& _NetworkInitData)
{
	// Insert To Data 
	m_NetworkInitDataContainer.insert(MakePair(_NetworkID, _NetworkInitData));
}

void UNetworkBaseController::InsertNetworkActorInContainer(_int _NetworkID, CSHPTRREF<UActor> _spActor)
{
	_spActor->SetNetworkID(_NetworkID);
	m_NetworkActorContainer.insert(MakePair(_NetworkID, _spActor));
}

SHPTR<UActor> UNetworkBaseController::FindNetworkActor(const _int _NetworkID)
{
	const auto& iter = m_NetworkActorContainer.find(_NetworkID);
	RETURN_CHECK(m_NetworkActorContainer.end() == iter, nullptr);
	return iter->second;
}

void UNetworkBaseController::SendTcpData(_char* _pData, short _tag, short _size)
{
	m_pSendOverExp->SendBufferReady(_pData, _tag, _size);
	UServerMethods::SendTcpPacket(m_ClientTcpSocket, m_pSendOverExp);
}

void UNetworkBaseController::SendProtoData(const UProcessedData& _ProcessedData)
{
	m_pSendOverExp->SendBufferReady(&_ProcessedData.GetData()[0], _ProcessedData.GetDataType(), _ProcessedData.GetDataSize());
	UServerMethods::SendTcpPacket(m_ClientTcpSocket, m_pSendOverExp);
}

void UNetworkBaseController::ServerTick()
{
	size_t numBytes{ 0 };
	_int result = UServerMethods::RecvTcpPacket(m_ClientTcpSocket, REF_OUT m_RecvTcpOverExp, REF_OUT numBytes);
	if (result != SOCKET_ERROR) {
		CombineRecvPacket(&m_RecvTcpOverExp, numBytes);
	}
}

void UNetworkBaseController::NativePacket()
{
	m_isNetworkTickRunning = true;
}

void UNetworkBaseController::CombineRecvPacket(UOverExp* _pOverExp, size_t _numBytes)
{
	if (0 >= _numBytes)
		return;

	// 버퍼를 조합한다. 
	{
		::memcpy(&m_TcpTotalBuffer[m_RemainBufferLength], _pOverExp->GetBufferAddress(0), _numBytes);
	}
	char* pBufferMove = &m_TcpTotalBuffer[0];
	size_t reaminBytes = _numBytes;
	// 만약 BufferLocation이 존재할 때
	while (reaminBytes > 0)
	{
		// PacketSize를 구한다. 
		PACKETHEAD PacketHead;
		memcpy(&PacketHead, pBufferMove, PACKETHEAD_SIZE);
		short CurrPacket = PacketHead.PacketSize + PACKETHEAD_SIZE;

		short Bytes = reaminBytes - CurrPacket;
		// 패킷의 현재 위치가 음수가 되는 경우면 
		if (0 > Bytes)
		{
			::memcpy(&m_TcpTotalBuffer[0], pBufferMove, reaminBytes);
			m_RemainBufferLength = reaminBytes;
			break;
		}
		ProcessPacket(&pBufferMove[PACKETHEAD_SIZE], PacketHead);
		// Buffer의 위치를 옮긴다. 
		reaminBytes -= CurrPacket;
		pBufferMove += CurrPacket;
	}
	m_RemainBufferLength = 0;
	::memset(&m_TcpTotalBuffer[0], 0, MAX_PROCESSBUF_LENGTH);
}

void UNetworkBaseController::InsertProcessedDataInActor(const UProcessedData& _ProcessedData)
{
	SHPTR<UActor> spActor = FindNetworkActor(_ProcessedData.GetDataID());
	if (nullptr != spActor)
	{
		spActor->ReceiveNetworkProcessData(_ProcessedData);
	}
	else
	{
		assert(nullptr != spActor);
	}
}

void UNetworkBaseController::ServerThread(void* _pData)
{
	UNetworkBaseController* pNetworkBaseController = static_cast<UNetworkBaseController*>(_pData);
	RETURN_CHECK(nullptr == pNetworkBaseController, ;);
	pNetworkBaseController->NativePacket();
	while (pNetworkBaseController->m_isNetworkTickRunning)
	{
		pNetworkBaseController->ServerTick();
	}
}

void UNetworkBaseController::Free()
{
	m_isNetworkTickRunning = false;
	delete m_pSendOverExp;
	closesocket(m_ClientTcpSocket);
	WSACleanup();
}
