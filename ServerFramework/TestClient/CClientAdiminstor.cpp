#include "ClientDefines.h"
#include "CClientAdiminstor.h"
#include "UCoreInstance.h"
#include "CClientSession.h"

CClientAdiminstor::CClientAdiminstor(OBJCON_CONSTRUCTOR) : 
	UClientService(OBJCON_CONDATA), m_iMaxSessionCount{500}
{
}

bool CClientAdiminstor::NativeConstruct()
{
	Connect();

	SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();

	for (_uint i = 0; i < TLS::MAX_THREAD; ++i)
	{
		spCoreInstance->RegisterFunc(CClientAdiminstor::ThreadFunc, this);
	}
	return true;
}

_bool CClientAdiminstor::Start()
{
	SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();
	spCoreInstance->Join();
	return true;
}

void CClientAdiminstor::Connect()
{
	SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();
	SHPTR<CClientAdiminstor> spClientAdiminstor = ThisShared<CClientAdiminstor>();
	for (_uint i = 0; i < m_iMaxSessionCount; ++i)
	{
		TCPSOCKET tcpSocket(GetIOContext());
		SESSIONID SessionID = GiveID();
		SHPTR<CClientSession> spClientSession = Create<CClientSession>(spCoreInstance, std::move(tcpSocket), spClientAdiminstor, SessionID);
		InsertSession(SessionID, spClientSession);
		spClientSession->ConnectTcpSocket();
	}
}

void CClientAdiminstor::Tick()
{
	GetIOContext().run();
}

void CClientAdiminstor::ThreadFunc(void* _Service)
{
	RETURN_CHECK(nullptr == _Service, ;);
	CClientAdiminstor* pService = static_cast<CClientAdiminstor*>(_Service);
	while (true)
	{
		pService->Tick();
	}
}

void CClientAdiminstor::Free()
{
}
