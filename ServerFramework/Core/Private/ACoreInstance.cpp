#include "CoreDefines.h"
#include "ACoreInstance.h"
#include "AThreadManager.h"
#include "ARandomManager.h"
#include "ASpaceManager.h"
#include "AService.h"

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

	/*
	-----------------------------
	CoreInstance
	-----------------------------
	UThreadManager
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
	UThreadManager
	-----------------------------
	URandomManager
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
	URandomManager
	-----------------------------
	USpaceManager
	-----------------------------
	*/

	void ACoreInstance::BuildGameSpace(const SPACEINFO& _SpaceInfo)
	{
		SHPTR<ASpaceManager> spSpaceManager = m_spSpaceManager;
		spSpaceManager->BuildGameSpace(_SpaceInfo);
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