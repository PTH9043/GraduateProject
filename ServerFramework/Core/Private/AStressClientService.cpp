#include "CoreDefines.h"
#include "AStressClientService.h"
#include "ASession.h"
#include "ACoreInstance.h"
namespace Core {

	AStressClientService::AStressClientService(OBJCON_CONSTRUCTOR) :
		AService(OBJCON_CONDATA, SERVICETYPE::CLIENT), 
		m_isRunningThread{true}
	{

	}

	_bool AStressClientService::NativeConstruct()	{
		return true;
	}

	_bool AStressClientService::Start()	{
		return __super::Start();
	}

	SHPTR<ASession> AStressClientService::FindSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor acces;
		const auto& iter = m_SessionContainer.find(acces, _SessionID);
		RETURN_CHECK(true == acces.empty(), nullptr);
		return acces->second;
	}

	void AStressClientService::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(0 >= m_SessionContainer.size(), ;);
		for (auto& iter : m_SessionContainer)
		{
			iter.second->WriteData(_pPacket, _PacketHead);
		}
	}

	void AStressClientService::LeaveService(const SESSIONID _SessionID)
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

	void AStressClientService::InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession)
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

	void AStressClientService::RunningThread(void* _pService)
	{
		RETURN_CHECK(nullptr == _pService, ;);
		AStressClientService* spClientService = static_cast<AStressClientService*>(_pService);
		while (true)
		{
			spClientService->Tick();
		}
	}

	SESSIONID AStressClientService::GiveID()
	{
		if (m_RemainIDQueue.empty())
		{
			return __super::GiveID();
		}
		SESSIONID SessionID{ 0 };
		m_RemainIDQueue.try_pop(SessionID);
		return SessionID;
	}

	void AStressClientService::RemoveDisconnectSockets()
	{
	}

	void AStressClientService::Free()
	{
		m_SessionContainer.clear();
	}

}