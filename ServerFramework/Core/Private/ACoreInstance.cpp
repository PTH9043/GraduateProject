#include "CoreDefines.h"
#include "ACoreInstance.h"
#include "AThreadManager.h"
#include "ARandomManager.h"
#include "ASpaceManager.h"
#include "AService.h"
#include "ANavigation.h"
#include "APathFinder.h"
#include "AMySqlDriver.h"
namespace Core
{
	ACoreInstance::ACoreInstance() :
		m_spService { nullptr},
		m_spThreadManager{ Create<Core::AThreadManager>() }, 
		m_spRandomManager{Create<Core::ARandomManager>()}, 
		m_spSpaceManager{ Create<Core::ASpaceManager>() }
	{

	}

	void ACoreInstance::ReadyCoreInstance(SHPTR<AService> _spService)
	{
		RETURN_CHECK(nullptr == _spService, ;);
		m_spService = _spService;
	//	m_spMySqlDriver = CreateInitConstructor<AMySqlDriver>(ThisShared<ACoreInstance>(), "tcp://127.0.0.1:3306", 
	//		"root", "Qkrxogus0652!");
		// Navigation Ready 
		for (auto& Navigation : m_NavigationWorkBench)
		{
			Navigation = CreateInitNative<ANavigation>("..\\..\\Resource\\Navigation\\interior.bin");
		}
		// PathFinder Ready
		for (_int i = 0; i < TLS::MAX_THREAD; ++i)
		{
			m_PathFinderWorkBench[i] = CreateInitNative<APathFinder>(*m_NavigationWorkBench[i]->GetCells().get());
		}

		//m_spPathFinder = CreateInitNative<APathFinder>(*m_spNavigation->GetCells().get());
	}

	_bool ACoreInstance::Start()
	{
		SHPTR<AService> spService = m_spService;
		return spService->Start();
	}

	SHPTR<ASession> ACoreInstance::FindSession(const SESSIONID _SessionID)
	{
		SHPTR<AService> spService = m_spService;
		return spService->FindSession(_SessionID);
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

	const SESSIONCONTAINER& ACoreInstance::GetSessionContainer() const
	{
		SHPTR<AService> spService = m_spService;
		ASSERT_CRASH(nullptr != spService);
		return spService->GetSessionContainer();
	}

	const GAMEOBJECTCONTAINER& ACoreInstance::GetGameObjectContainer() const
	{
		SHPTR<AService> spService = m_spService;
		ASSERT_CRASH(nullptr != spService);
		return spService->GetGameObjectContainer();
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

	/*
	-----------------------------
	AMySqlDriver
	-----------------------------
	ANavigation
	-----------------------------
	*/

	const _float ACoreInstance::ComputeHeight(const Vector3& _vPosition)
	{
		return m_NavigationWorkBench[TLS::g_ThreadID]->ComputeHeight(_vPosition);
	}

	void ACoreInstance::ComputeHeight(CSHPTRREF<ATransform> _spTransform)
	{
		m_NavigationWorkBench[TLS::g_ThreadID]->ComputeHeight(_spTransform);
	}

	_bool ACoreInstance::IsMove(Vector3 _vPosition, SHPTR<ACell>& _spCell)
	{
		return m_NavigationWorkBench[TLS::g_ThreadID]->IsMove(_vPosition, _spCell);
	}

	_bool ACoreInstance::IsMove(Vector3 _vPosition)
	{
		SHPTR<ACell> spCell;
		return m_NavigationWorkBench[TLS::g_ThreadID]->IsMove(_vPosition, spCell);
	}

	_bool ACoreInstance::IsMove(_int _iCurOnCellIndex, Vector3 _vPosition, SHPTR<ACell>& _spCell)
	{
		return m_NavigationWorkBench[TLS::g_ThreadID]->IsMove(_iCurOnCellIndex, _vPosition, _spCell);
	}

	_bool ACoreInstance::IsMove(_int _iCurOnCellIndex, Vector3 _vPosition)
	{
		SHPTR<ACell> spCell;
		return m_NavigationWorkBench[TLS::g_ThreadID]->IsMove(_iCurOnCellIndex, _vPosition, spCell);
	}

	SHPTR<ACell> ACoreInstance::FindCell(const Vector3& _vPosition)
	{
		return m_NavigationWorkBench[TLS::g_ThreadID]->FindCell(_vPosition);
	}

	SHPTR<ACell> ACoreInstance::FindCellWithoutUpdate(const Vector3& _vPosition)
	{
		return m_NavigationWorkBench[TLS::g_ThreadID]->FindCellWithoutUpdate(_vPosition);
	}

	SHPTR<ACell> ACoreInstance::FindCell(const _int& _iIndex)
	{
		return m_NavigationWorkBench[TLS::g_ThreadID]->FindCell(_iIndex);
	}

	SHPTR<ACell> ACoreInstance::FindCellWithoutUpdate(const _int& _iIndex)
	{
		return m_NavigationWorkBench[TLS::g_ThreadID]->FindCellWithoutUpdate(_iIndex);
	}

	/*
	--------------------------------------
	ANavigation
	--------------------------------------
	APathFinder
	--------------------------------------
	*/

	void ACoreInstance::FindPath(Vector3 _vStartPos, Vector3 _vEndPos)
	{
		m_PathFinderWorkBench[TLS::g_ThreadID]->FindPath(m_NavigationWorkBench[TLS::g_ThreadID], _vStartPos, _vEndPos);
	}

	LIST<Vector3>& ACoreInstance::GetBestList()
	{
		ASSERT_CRASH(nullptr != m_PathFinderWorkBench[TLS::g_ThreadID])
		return m_PathFinderWorkBench[TLS::g_ThreadID]->GetBestList();
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