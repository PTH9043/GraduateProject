#include "ServerDefines.h"
#include "CServerAdiminstor.h"
#include "CPlayerSession.h"
#include "UCoreInstance.h"

namespace Server
{
	CServerAdiminstor::CServerAdiminstor(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath) :
		Core::UServerService(OBJCON_CONDATA, _strNavigationPath)
	{
	}

	bool CServerAdiminstor::NativeConstruct()
	{
		Connect();
		return __super::NativeConstruct();
	}

	bool CServerAdiminstor::Start()
	{
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
	}
}