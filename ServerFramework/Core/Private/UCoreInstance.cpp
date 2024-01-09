#include "CoreDefines.h"
#include "UCoreInstance.h"
#include "UThreadManager.h"
#include "URandomManager.h"
#include "ULogManager.h"

namespace Core
{
	UCoreInstance::UCoreInstance() : 
		m_spThreadManager{ Create<Core::UThreadManager>() }, 
		m_spRandomManager{Create<Core::URandomManager>()}, 
		m_spLogManager{Create<Core::ULogManager>() }
	{

	}

	bool UCoreInstance::NativeConstruct()
	{

		return S_OK;
	}

	/*
	-----------------------------
	CoreGrobal
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
	UThreadManager
	-----------------------------
	ULogManager
	-----------------------------
	*/

	void UCoreInstance::Free()
	{
		LOCKGUARD<MUTEX> Lock{ m_Mutex };
		using namespace std;
		std::this_thread::sleep_for(100ms);

		m_spThreadManager.reset();
		m_spRandomManager.reset();
	}
}