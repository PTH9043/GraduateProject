#include "ServerDefines.h"
#include "CServerAdiminstor.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "CMonsterJobTimer.h"
#include "APathJobTimer.h"
#include "CMummy.h"
#include "CMobLayoutLoader.h"
#include "CSarcophagus.h"

namespace Server
{
	CServerAdiminstor::CServerAdiminstor(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath) :
		Core::AServerService(OBJCON_CONDATA, _strNavigationPath)
	{
	}

	bool CServerAdiminstor::NativeConstruct()
	{
		Connect();
		return __super::NativeConstruct();
	}

	bool CServerAdiminstor::Start()
	{
		GetCoreInstance()->RegisterJob(TIMERTYPE::TIMER_MOB, Create<CMonsterJobTimer>(GetCoreInstance(), GetIOContext()));
		GetCoreInstance()->RegisterJob(TIMERTYPE::TIMER_ASTAR, Create<APathJobTimer>(GetCoreInstance(), GetIOContext()));

		CMobLayoutLoader MummyStandingLayout("..\\..\\Resource\\MobsLayouts\\Mummy_Standing.bin");
		// Standing
		{
			SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
			for (auto& iter : MummyStandingLayout.GetMobData())
			{
				CreateMobObject(&iter, SARCO_STANDING);
			}
		}
		CMobLayoutLoader MummylayingLayout("..\\..\\Resource\\MobsLayouts\\Mummy_Laying.bin");
		// Standing
		{
			SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
			for (auto& iter : MummylayingLayout.GetMobData())
			{
				CreateMobObject(&iter, SARCO_LAYING);
			}
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
				Core::SHPTR<Core::ACoreInstance> spCoreInstance = GetCoreInstance();
				Core::SHPTR<Core::ASession> spSession = Core::Create<CPlayerSession>(spCoreInstance, std::move(TcpSocket), id);
				// Insert 
				InsertSession(id, spSession);
				spSession->Start();
			}
			Connect();
			});
	}

	void CServerAdiminstor::CreateMobObject(void* _pData, SARCOPHAGUSTYPE _SarcophagusType)
	{
		SHPTR<CSarcophagus> spSarcophagus = Create<CSarcophagus>(GetCoreInstance(), GiveID(), _SarcophagusType);
		SESSIONID MummyID = GiveID();
		spSarcophagus->Start(VOIDDATAS{ _pData, &MummyID });
		InsertMobObject(spSarcophagus->GetSessionID(), spSarcophagus);
	}

	void CServerAdiminstor::Free()
	{
	}
}