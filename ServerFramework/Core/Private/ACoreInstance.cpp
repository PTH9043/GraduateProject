#include "CoreDefines.h"
#include "ACoreInstance.h"
#include "AThreadManager.h"
#include "ARandomManager.h"
#include "ASpaceManager.h"
#include "ANavigation.h"
#include "AMySqlDriver.h"
#include "AServerService.h"
#include "APawn.h"
#include "ASession.h"

namespace Core
{
	ACoreInstance::ACoreInstance() :
		m_spService{ nullptr },
		m_spThreadManager{ Create<Core::AThreadManager>() },
		m_spRandomManager{ Create<Core::ARandomManager>() },
		m_spSpaceManager{ Create<Core::ASpaceManager>() }
	{

	}

	void ACoreInstance::ReadyCoreInstance(SHPTR<AService> _spService)
	{
		RETURN_CHECK(nullptr == _spService, ;);
		m_spService = _spService;

	//	m_spMySqlDriver = CreateInitConstructor<AMySqlDriver>(ThisShared<ACoreInstance>(), "tcp://127.0.0.1:3306", 
	//		"root", "Qkrxogus0652!");

		m_spNavigation = CreateInitNative<ANavigation>("..\\..\\Resource\\Navigation\\interior.bin");
	}

	_bool ACoreInstance::Start()
	{
		SHPTR<AService> spService = m_spService;
		m_spThreadManager->CreateMainLoop(ThisShared<ACoreInstance>(), m_spService->GetIOContext(), m_spService);
		return spService->Start();
	}

	SHPTR<ASession> ACoreInstance::FindSession(const SESSIONID _SessionID)
	{
		SHPTR<AService> spService = m_spService;
		return spService->FindSession(_SessionID);
	}

	SHPTR<AMonster> ACoreInstance::FindMobObject(const SESSIONID _SessionID)
	{
		SHPTR<AService> spService = m_spService;
		return spService->FindMobObject(_SessionID);
	}

	SHPTR<AStaticObject> ACoreInstance::FindStaticObject(const SESSIONID _SessionID)
	{
		SHPTR<AService> spService = m_spService;
		return spService->FindStaticObject(_SessionID);
	}

	void ACoreInstance::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		SHPTR<AService> spService = m_spService;
		spService->BroadCastMessage(_pPacket, _PacketHead);
	}

	void ACoreInstance::BroadCastMessageExcludingSession(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		SHPTR<AService> spService = m_spService;
		spService->BroadCastMessageExcludingSession(_SessionID, _pPacket, _PacketHead);
	}

	void ACoreInstance::DirectSendMessage(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		SHPTR<AService> spService = m_spService;
		spService->DirectSendMessage(_SessionID, _pPacket, _PacketHead);
	}

	void ACoreInstance::LeaveService(const SESSIONID _SessionID)
	{
		SHPTR<AService> spService = m_spService;
		spService->LeaveService(_SessionID);
	}

	void ACoreInstance::InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession)
	{
		SHPTR<AService> spService = m_spService;
		spService->InsertSession(_SessionID, _spSession);
	}

	void ACoreInstance::InsertMobObject(SESSIONID _SessionID, SHPTR<AMonster> _spMobObject)
	{
		SHPTR<AService> spService = m_spService;
		spService->InsertMobObject(_SessionID, _spMobObject);
	}

	void ACoreInstance::InsertStaticObj(SESSIONID _SessionID, SHPTR<AStaticObject> _spStaticObj)
	{
		SHPTR<AService> spService = m_spService;
		spService->InsertStaticObj(_SessionID, _spStaticObj);
	}

	const SESSIONCONTAINER& ACoreInstance::GetSessionContainer() const
	{
		SHPTR<AService> spService = m_spService;
		ASSERT_CRASH(nullptr != spService);
		return spService->GetSessionContainer();
	}

	const MOBOBJCONTAINER& ACoreInstance::GetMobObjContainer() const
	{
		SHPTR<AService> spService = m_spService;
		ASSERT_CRASH(nullptr != spService);
		return spService->GetMobObjContainer();
	}

	const STATICOBJCONTAINER& ACoreInstance::GetStaticObjContainer() const
	{
		SHPTR<AService> spService = m_spService;
		ASSERT_CRASH(nullptr != spService);
		return spService->GetStaticObjContainer();
	}

	SHPTR<AServerService> ACoreInstance::GetServerService()
	{
		return Core::dynamic_shared_cast<AServerService>(m_spService);
	}

	/*
	-----------------------------
	ACoreInstance
	-----------------------------
	AThreadManager
	-----------------------------
	*/

	void ACoreInstance::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		m_spThreadManager->RegisterFunc(_CallBack, _Data);
	}

	void ACoreInstance::RegisterJob(_int _jobType, CSHPTRREF<AJobTimer> _spJobTimer)
	{
		m_spThreadManager->RegisterJob(_jobType, _spJobTimer);
	}

	void ACoreInstance::InsertPawnCollisionList(AGameObject* _pGameObject)
	{
		m_spThreadManager->InsertPawnCollisionList(_pGameObject);
	}

	void ACoreInstance::InsertStaticObjCollisionList(AGameObject* _pGameObject)
	{
		m_spThreadManager->InsertStaticObjCollisionList(_pGameObject);
	}

	SHPTR<AJobTimer> ACoreInstance::FindJobTimer(_int _JobTimer)
	{
		return m_spThreadManager->FindJobTimer(_JobTimer);
	}

	void ACoreInstance::Join()
	{
		m_spThreadManager->Join();
	}


	/*
	-----------------------------
	AThreadManager
	-----------------------------
	ARandomManager
	-----------------------------
	*/


	_int ACoreInstance::ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum)
	{
		SHPTR<ARandomManager> spRandomManager = m_spRandomManager;
		return spRandomManager->ReturnRadomNumber(_iMinNum, _iMaxNum);
	}

	_int ACoreInstance::ReturnRadomNumber(const _int _iMaxNum)
	{
		SHPTR<ARandomManager> spRandomManager = m_spRandomManager;
		return spRandomManager->ReturnRadomNumber(_iMaxNum);
	}

	/*
	-----------------------------
	ARandomManager
	-----------------------------
	ASpaceManager
	-----------------------------
	*/

	void ACoreInstance::BuildGameSpace(const SPACEINFO& _SpaceInfo)
	{
		SHPTR<ASpaceManager> spSpaceManager = m_spSpaceManager;
		spSpaceManager->BuildGameSpace(_SpaceInfo);
	}

	/*
	-----------------------------
	ASpaceManager
	-----------------------------
	AMySqlDriver
	-----------------------------
	*/

	_bool ACoreInstance::ExcuteQueryMessage(SQLTABLETYPE _TableType, SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		return spMySqlDriver->ExcuteQueryMessage(_TableType, _sqlQueryType, _strQueryData);
	}

	void ACoreInstance::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _bool _Value)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		spMySqlDriver->BindParam(_TableType, _ParamIndex, _Value);
	}

	void ACoreInstance::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _float _Value)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		spMySqlDriver->BindParam(_TableType, _ParamIndex, _Value);
	}

	void ACoreInstance::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _double _Value)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		spMySqlDriver->BindParam(_TableType, _ParamIndex, _Value);
	}

	void ACoreInstance::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _short _Value)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		spMySqlDriver->BindParam(_TableType, _ParamIndex, _Value);
	}

	void ACoreInstance::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _int _Value)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		spMySqlDriver->BindParam(_TableType, _ParamIndex, _Value);
	}

	void ACoreInstance::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _llong _Value)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		spMySqlDriver->BindParam(_TableType, _ParamIndex, _Value);
	}

	void ACoreInstance::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, const _string& _Value)
	{
		SHPTR<AMySqlDriver> spMySqlDriver = m_spMySqlDriver;
		spMySqlDriver->BindParam(_TableType, _ParamIndex, _Value);
	}

	SHPTR<ANavigation> ACoreInstance::CloneNavi()
	{
		return Create<ANavigation>(*m_spNavigation.get());
	}

	void ACoreInstance::CheckAllPlayerDie()
	{
		const auto& SessionContainer = GetSessionContainer();
		for (auto& value : SessionContainer)
		{
			if (false == value.second->IsDead())
			{
				return;
			}
		}
		m_isCheckAllPlayerDie = true;
	}

	void ACoreInstance::Free()
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		m_spService.reset();
		m_spSpaceManager.reset();
		m_spRandomManager.reset();
		m_spThreadManager.reset();
	}
}