#include "ServerDefines.h"
#include "CServerAdiminstor.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "CMonsterJobTimer.h"
#include "APathJobTimer.h"
#include "CMummy.h"

namespace Server
{
	CServerAdiminstor::CServerAdiminstor(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath) :
		Core::AServerService(OBJCON_CONDATA, _strNavigationPath)
	{
	}

	bool CServerAdiminstor::NativeConstruct()
	{
		GetCoreInstance()->RegisterJob(TIMERTYPE::TIMER_MOB, Create<CMonsterJobTimer>(GetCoreInstance(), GetIOContext()));
		GetCoreInstance()->RegisterJob(TIMERTYPE::TIMER_ASTAR, Create<APathJobTimer>(GetCoreInstance(), GetIOContext()));
		Connect();
		return __super::NativeConstruct();
	}

	bool CServerAdiminstor::Start()
	{
		SHPTR<CMummy> spMummy = Create<CMummy>(GetCoreInstance(), GiveID());

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
				Core::SHPTR<Core::ACoreInstance> spCoreInstance = GetCoreInstance();
				Core::SHPTR<Core::ASession> spSession = Core::Create<CPlayerSession>(spCoreInstance, std::move(TcpSocket), id);
				// Insert 
				InsertSession(id, spSession);
				spSession->Start();
			}
			Connect();
			});
	}
	void CServerAdiminstor::Free()
	{
	}
}