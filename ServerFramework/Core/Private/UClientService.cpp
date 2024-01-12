#include "CoreDefines.h"
#include "UClientService.h"
#include "USession.h"
#include "UCoreInstance.h"
namespace Core {

	UClientService::UClientService(OBJCON_CONSTRUCTOR) : 
		UService(OBJCON_CONDATA, SERVICETYPE::CLIENT), 
		m_isRunningThread{true}
	{

	}

	_bool UClientService::NativeConstruct()	{
		return true;
	}

	_bool UClientService::Start()	{
		return __super::Start();
	}

	SHPTR<USession> UClientService::FindSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor acces;
		const auto& iter = m_SessionContainer.find(acces, _SessionID);
		RETURN_CHECK(true == acces.empty(), nullptr);
		return acces->second;
	}

	void UClientService::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(0 >= m_SessionContainer.size(), ;);
		for (auto& iter : m_SessionContainer)
		{
			iter.second->WriteData(_pPacket, _PacketHead);
		}
	}

	void UClientService::LeaveService(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor acces;
		const auto& iter = m_SessionContainer.find(acces, _SessionID);
		RETURN_CHECK(true == acces.empty(), ;);
		// Disconnect
		{
		//	std::atomic_thread_fence(std::memory_order_seq_cst);
			m_RemainIDQueue.push(acces->first);
			m_SessionContainer.erase(acces);
		}
	}

	void UClientService::InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		if (nullptr == FindSession(_SessionID))
		{
			m_SessionContainer.emplace(MakePair(_SessionID, _spSession));
		}
		else
		{
			SESSIONCONTAINER::accessor acces;
			const auto& iter = m_SessionContainer.find(acces, _SessionID);
			acces->second = _spSession;
		}
	}

	void UClientService::RunningThread(void* _pService)
	{
		RETURN_CHECK(nullptr == _pService, ;);
		UClientService* spClientService = static_cast<UClientService*>(_pService);
		while (true)
		{
			spClientService->Tick();
		}
	}

	SESSIONID UClientService::GiveID()
	{
		if (m_RemainIDQueue.empty())
		{
			return __super::GiveID();
		}
		SESSIONID SessionID{ 0 };
		m_RemainIDQueue.try_pop(SessionID);
		return SessionID;
	}

	void UClientService::RemoveDisconnectSockets()
	{
	}

	void UClientService::Free()
	{
		m_SessionContainer.clear();
	}

}