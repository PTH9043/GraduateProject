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
		return true;
	}

	SHPTR<USession> UClientService::FindSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor acces;
		const auto& iter = m_SessionContainer.find(acces, _SessionID);
		RETURN_CHECK(false == acces.empty(), nullptr);
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
		RETURN_CHECK(false == acces.empty(), ;);
		// Disconnect
		acces->second->Disconnect();
	}

	void UClientService::RemoveService(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor acces;
		const auto& iter = m_SessionContainer.find(acces, _SessionID);
		RETURN_CHECK(false == acces.empty(), ;);
		acces->second = nullptr;
		__super::LeaveService(_SessionID);
	}

	void UClientService::InsertSession(SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		SESSIONID SessionID = _spSession->GetID();
		m_SessionContainer.emplace(MakePair(SessionID, _spSession));
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

	void UClientService::Free()
	{
		m_SessionContainer.clear();
	}

}