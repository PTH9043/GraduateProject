#include "CoreDefines.h"
#include "UServerService.h"
#include "USession.h"
#include "UCoreInstance.h"

namespace Core {

	UServerService::UServerService(OBJCON_CONSTRUCTOR) : 
		UService(OBJCON_CONDATA, SERVICETYPE::SERVER)
	{
	}

	_bool UServerService::NativeConstruct()
	{
		AsyncAccept();

		SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();

		for (_uint i = 0; i < TLS::MAX_THREAD; ++i)
		{
			spCoreInstance->RegisterFunc(UServerService::ThreadFunc, this);
		}
		return true;
	}

	_bool UServerService::Start()
	{
		SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();
		spCoreInstance->Join();
		return true;
	}

	SHPTR<USession> UServerService::FindSession(const SESSIONID _SessionID)
	{
		const auto& iter = m_SessionContainer.find(_SessionID);
		RETURN_CHECK(iter == m_SessionContainer.end(), nullptr);
		return iter->second;
	}

	void UServerService::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(0 >= GetCurSessionCount(), ;);
		for (auto& iter : m_SessionContainer)
		{
			iter.second->WriteData(_pPacket, _PacketHead);
		}
	}

	void UServerService::LeaveService(const SESSIONID _SessionID)
	{
		const auto& iter = m_SessionContainer.find(_SessionID);
		RETURN_CHECK(iter == m_SessionContainer.end(), ;);
		// Disconnect
		iter->second->Disconnect();
	}

	void UServerService::RemoveService(const SESSIONID _SessionID)
	{
		const auto& iter = m_SessionContainer.find(_SessionID);
		RETURN_CHECK(iter == m_SessionContainer.end(), ;);
		iter->second = nullptr;
		__super::LeaveService(_SessionID);
	}

	void UServerService::InsertSession(SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		SESSIONID SessionID = GiveID();
		m_SessionContainer.emplace(MakePair(SessionID, _spSession));
		__super::InsertSession(_spSession);
	}

	void UServerService::ThreadFunc(void* _spServerService)
	{
		RETURN_CHECK(nullptr == _spServerService, ;);
		UServerService* pServerService = static_cast<UServerService*>(_spServerService);
		IOContext& context = pServerService->GetIOContext(REF_RETURN);
		// Running 
		while (pServerService->m_isConinueThread)
		{
			// 실행 
			context.run();
			// 작업이 없으면 해당 스레드를 쉬게 둔다.
			std::this_thread::yield();
		}
	}

	void UServerService::Free()
	{
		for (auto& iter : m_SessionContainer)
		{
			iter.second->Disconnect();
		}
	}
}