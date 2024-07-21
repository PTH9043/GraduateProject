#include "ServerDefines.h"
#include "CServerAdiminstor.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "CMonsterJobTimer.h"
#include "CMummy.h"
#include "CMobLayoutLoader.h"
#include "CSarcophagus.h"
#include "CMobLayOutSaver.h"
#include "CChest.h"
#include "ATransform.h"
#include "AAnimController.h"

//#define CREATED_SERVERMOBDATA

namespace Server
{
	CServerAdiminstor::CServerAdiminstor(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath) :
		Core::AServerService(OBJCON_CONDATA, _strNavigationPath)
	{
	}

	bool CServerAdiminstor::NativeConstruct()
	{
		return __super::NativeConstruct();
	}

	bool CServerAdiminstor::Start()
	{
#ifndef CREATED_SERVERMOBDATA
		CreateServerMobResourceData();
#else
		CreateServerMob();
#endif
		ThreadMiliRelax(100);
		Connect();
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

	void CServerAdiminstor::CreateServerMobResourceData()
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AJobTimer> spMonsterJobTimer = Create<CMonsterJobTimer>(GetCoreInstance(), GetIOContext());
		spCoreInstance->RegisterJob(TIMERTYPE::TIMER_MOB, spMonsterJobTimer);
		// Standing
		{
			VECTOR< MOBDATA> MobData;
			CMobLayoutLoader MummyStandingLayout("..\\..\\Resource\\MobsLayouts\\Mummy_Standing.bin");
			CMobLayoutLoader MummylayingLayout("..\\..\\Resource\\MobsLayouts\\Mummy_Laying.bin");
			MobData.reserve(MummyStandingLayout.GetMobData().size() + MummylayingLayout.GetMobData().size());
			// Mob데이터를 받아온다. 
			for (auto& iter : MummyStandingLayout.GetMobData())
			{
				MobData.push_back(iter);
			}
			for (auto& iter : MummylayingLayout.GetMobData())
			{
				MobData.push_back(iter);
			}
			// Mob 데이터를 상자로 만든다. 
			for (auto& iter : MobData)
			{
				if (iter.strAnimModelName == "Chest_FBX.bin")
				{
					CreateChest(&iter, spMonsterJobTimer);
				}
				else if (iter.strAnimModelName == "Mummy_DEMO_1_FBX.bin")
				{
					if (iter.strAnimName == "staticLaying")
					{
						CreateMummyAndSarphagousMob(&iter, SARCO_LAYING, spMonsterJobTimer);
					}
					else
					{
						CreateMummyAndSarphagousMob(&iter, SARCO_STANDING, spMonsterJobTimer);
					}
				}
			}
			// Monster 
			{
				const auto& GameObjectList = spCoreInstance->GetMobObjContainer();
				VECTOR<MOBSERVERDATA> MobData;
				MobData.reserve(GameObjectList.size());

				for (auto& iter : GameObjectList)
				{
					MOBSERVERDATA data;
					data.mWorldMatrix = iter.second->GetTransform()->GetWorldMatrix();
					data.iMobID = iter.second->GetSessionID();
					data.iMobType = iter.second->GetMonsterType();
					data.iStartAnimIndex = iter.second->GetAnimController()->GetCurAnimIndex();
					MobData.push_back(data);
				}
				CMobLayOutSaver saver("..\\..\\NetworkMobData.bin", MobData);
			}
		}
	}

	void CServerAdiminstor::CreateServerMob()
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AJobTimer> spMonsterJobTimer = Create<CMonsterJobTimer>(GetCoreInstance(), GetIOContext());
		spCoreInstance->RegisterJob(TIMERTYPE::TIMER_MOB, spMonsterJobTimer);

		CMobServerLayoutLoader MobServerLayoutLoader("..\\..\\Resource\\NetworkMobData.bin");
		ThreadMiliRelax(100);
		for (auto& iter : MobServerLayoutLoader.GetMobData())
		{
			SHPTR<AMonster> spMonster = nullptr;

			switch (iter.iMobType)
			{
			case TAG_CHEST:
				spMonster = Create<CChest>(GetCoreInstance(), iter.iMobID, spMonsterJobTimer);
				break;
			case TAG_MUMMY_LAYING:
				spMonster = Create<CMummy>(GetCoreInstance(), iter.iMobID, MUMMYTYPE::MUMMY_LAYING, spMonsterJobTimer);
				break;
			case TAG_MUMMY_STANDING:
				spMonster = Create<CMummy>(GetCoreInstance(), iter.iMobID, MUMMYTYPE::MUMMY_STANDING, spMonsterJobTimer);
				break;
			}

			if (nullptr == spMonster)
				continue;

			spCoreInstance->InsertMobObject(spMonster->GetSessionID(), spMonster);
			spMonster->Start({ &iter });
		}
		IncreaseCurrentSessionCount(MobServerLayoutLoader.GetMobData().size());
	}

	void CServerAdiminstor::CreateMummyAndSarphagousMob(void* _pData, SARCOPHAGUSTYPE _SarcophagusType, SHPTR<AJobTimer> _spMonsterJobTimer)
	{
		SHPTR<CSarcophagus> spSarcophagus = Create<CSarcophagus>(GetCoreInstance(), GiveID(), _SarcophagusType, _spMonsterJobTimer);
		SESSIONID MummyID = GiveID();
		spSarcophagus->Start(VOIDDATAS{ _pData, &MummyID });
		InsertMobObject(spSarcophagus->GetSessionID(), spSarcophagus);
	}

	void CServerAdiminstor::CreateChest(void* _pData, SHPTR<AJobTimer> _spMonsterJobTimer)
	{
		SHPTR<CChest> spChest = Create<CChest>(GetCoreInstance(), GiveID(), _spMonsterJobTimer);
		spChest->Start(VOIDDATAS{ _pData });
		InsertMobObject(spChest->GetSessionID(), spChest);
	}

	void CServerAdiminstor::Free()
	{
	}
}