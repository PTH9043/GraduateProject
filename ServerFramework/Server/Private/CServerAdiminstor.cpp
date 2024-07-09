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
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AJobTimer> spMonsterJobTimer = Create<CMonsterJobTimer>(GetCoreInstance(), GetIOContext());
		spCoreInstance->RegisterJob(TIMERTYPE::TIMER_MOB, spMonsterJobTimer);
		spCoreInstance->RegisterJob(TIMERTYPE::TIMER_ASTAR, Create<APathJobTimer>(GetCoreInstance(), GetIOContext()));

		CMobLayoutLoader MummyStandingLayout("..\\..\\Resource\\MobsLayouts\\Mummy_Standing.bin");
		// Standing
		{
			for (auto& iter : MummyStandingLayout.GetMobData())
			{
				CreateMummyAndSarphagousMob(&iter, SARCO_STANDING, spMonsterJobTimer);
			}
		}
		CMobLayoutLoader MummylayingLayout("..\\..\\Resource\\MobsLayouts\\Mummy_Laying.bin");
		// Standing
		{
			SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
			for (auto& iter : MummylayingLayout.GetMobData())
			{
				CreateMummyAndSarphagousMob(&iter, SARCO_LAYING, spMonsterJobTimer);
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

	void CServerAdiminstor::CreateMummyAndSarphagousMob(void* _pData, SARCOPHAGUSTYPE _SarcophagusType, SHPTR<AJobTimer> _spMonsterJobTimer)
	{
		SHPTR<CSarcophagus> spSarcophagus = Create<CSarcophagus>(GetCoreInstance(), GiveID(), _SarcophagusType, _spMonsterJobTimer);
		SESSIONID MummyID = GiveID();
		spSarcophagus->Start(VOIDDATAS{ _pData, &MummyID });
		InsertMobObject(spSarcophagus->GetSessionID(), spSarcophagus);
	}

	void CServerAdiminstor::Free()
	{
	}
}