#include "ServerDefines.h"
#include "CServerAdiminstor.h"

namespace Server
{
	CServerAdiminstor::CServerAdiminstor(OBJCON_CONSTRUCTOR) :
		Core::UServerService(OBJCON_CONDATA)
	{
	}

	void CServerAdiminstor::AsyncAccept()
	{
		TCPACCEPTOR& Acceptor = GetTcpAccepctor();
		TCPSOCKET& TcpSocket = GetTcpSocket();
	}

	void CServerAdiminstor::Free()
	{
	}
}