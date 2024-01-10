#include "ServerDefines.h"
#include "CServerAdiminstor.h"
#include "CPlayerSession.h"
#include "UCoreInstance.h"
#include "CAwsServerConnector.h"

namespace Server
{
	CServerAdiminstor::CServerAdiminstor(OBJCON_CONSTRUCTOR, _bool _CreateAwsConnector) :
		Core::UServerService(OBJCON_CONDATA), m_spAwsConnector{nullptr}
	{
		if (true == _CreateAwsConnector)
		{
			m_spAwsConnector = Create<CAwsServerConnector>(GetCoreInstance());
		}
	}

	bool CServerAdiminstor::NativeConstruct()
	{
		Connect();
		return __super::NativeConstruct();
	}

	bool CServerAdiminstor::Start()
	{
		if(nullptr != m_spAwsConnector)
		{
			m_spAwsConnector->Start();
		}
		return __super::Start();
	}

	void CServerAdiminstor::Connect()
	{
		TCPACCEPTOR& TcpAcceptor = GetTcpAccepctor();
		TCPSOCKET& TcpSocket = GetTcpSocket();
		TcpAcceptor.async_accept(TcpSocket, [&](const boost::system::error_code& _error) {
			if (!_error)
			{
				SESSIONID id = GiveID();
				Core::SHPTR<Core::UCoreInstance> spCoreInstance = GetCoreInstance();

				Core::SHPTR<Core::USession> spSession =
					Core::Create<CPlayerSession>(spCoreInstance, std::move(TcpSocket), ThisShared<CServerAdiminstor>(), id);

				// Insert 
				InsertSession(id, spSession);

#ifdef USE_DEBUG
		//		std::cout << "Accept Success [" << id << "]\n";
#endif 
				spSession->Start();
			}
			Connect();
			});
	}

	void CServerAdiminstor::Free()
	{
		m_spAwsConnector.reset();
	}
}