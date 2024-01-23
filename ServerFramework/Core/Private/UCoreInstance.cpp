#include "CoreDefines.h"
#include "UCoreInstance.h"
#include "UThreadManager.h"
#include "URandomManager.h"
#include "ULogManager.h"
#include "USpaceManager.h"
#include "UService.h"

namespace Core
{
	UCoreInstance::UCoreInstance() :
		m_spService { nullptr},
		m_spThreadManager{ Create<Core::UThreadManager>() }, 
		m_spRandomManager{Create<Core::URandomManager>()}, 
		m_spLogManager{Create<Core::ULogManager>() },
		m_spSpaceManager{ Create<Core::USpaceManager>() }
	{

	}

	void UCoreInstance::ReadyCoreInstance(SHPTR<UService> _spService)
	{
		RETURN_CHECK(nullptr == _spService, ;);
		m_spService = _spService;
	}

	void UCoreInstance::Start()
	{
		m_spService->Start();
	}

	SHPTR<USession> UCoreInstance::FindSession(const SESSIONID _SessionID)
	{
		return m_spService->FindSession(_SessionID);
	}

	void UCoreInstance::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		m_spService->BroadCastMessage(_pPacket, _PacketHead);
	}

	void UCoreInstance::LeaveService(const SESSIONID _SessionID)
	{
		m_spService->LeaveService(_SessionID);
	}

	void UCoreInstance::InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession)
	{
		m_spService->InsertSession(_SessionID, _spSession);
	}

	/*
	-----------------------------
	CoreInstance
	-----------------------------
	UThreadManager
	-----------------------------
	*/

	void UCoreInstance::RegisterFunc(const THREADFUNC& _CallBack, void* _Data)
	{
		m_spThreadManager->RegisterFunc(_CallBack, _Data);
	}

	void UCoreInstance::Join()
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


	_int UCoreInstance::ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum)
	{
		return m_spRandomManager->ReturnRadomNumber(_iMinNum, _iMaxNum);
	}

	_int UCoreInstance::ReturnRadomNumber(const _int _iMaxNum)
	{
		return m_spRandomManager->ReturnRadomNumber(_iMaxNum);
	}

	/*
	-----------------------------
	UThreadManager
	-----------------------------
	ULogManager
	-----------------------------
	*/

	void UCoreInstance::PrintOut(const char* _fmt, ...)
	{
		va_list args;
		va_start(args, _fmt);
		vsprintf_s(TLS::g_LogTextBuffer, _fmt, args);
		va_end(args);

		m_spLogManager->PrintOut(TLS::g_LogTextBuffer);
	}

	void UCoreInstance::FileOut(const char* _fmt, ...)
	{
		va_list args;
		va_start(args, _fmt);
		vsprintf_s(TLS::g_LogTextBuffer, _fmt, args);
		va_end(args);

		m_spLogManager->FileOut(TLS::g_LogTextBuffer);
	}

	/*
	-----------------------------
	ULogManager
	-----------------------------
	USpaceManager
	-----------------------------
	*/

	void UCoreInstance::BuildGameSpace(const SPACEINFO& _SpaceInfo)
	{
		m_spSpaceManager->BuildGameSpace(_SpaceInfo);
	}

	void UCoreInstance::Free()
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		using namespace std;
		std::this_thread::sleep_for(100ms);
		m_spSpaceManager.reset();
		m_spRandomManager.reset();
		m_spLogManager.reset();
		m_spThreadManager.reset();
	}
}