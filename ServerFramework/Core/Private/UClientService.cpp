#include "CoreDefines.h"
#include "UClientService.h"
#include "USession.h"

namespace Core {

	UClientService::UClientService(OBJCON_CONSTRUCTOR) : 
		UService(OBJCON_CONDATA, SERVICETYPE::CLIENT)
	{
	}

	SHPTR<USession> UClientService::FindSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor accessor;
		RETURN_CHECK(false == m_SessionContainer.find(accessor, _SessionID), nullptr);
		return accessor->second;
	}

	void UClientService::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(0 >= GetCurSessionCount(), ;);
		for (auto& iter : m_SessionContainer)
		{
			iter.second->WriteData(_pPacket, _PacketHead);
		}
	}

	void UClientService::LeaveService(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor accessor;
		RETURN_CHECK(false == m_SessionContainer.find(accessor, _SessionID), ;);

		accessor->second->Disconnect();
		m_SessionContainer.erase(accessor);
		__super::LeaveService(_SessionID);

		m_RemainIDStack.Push(_SessionID);
	}

	void UClientService::InsertSession(SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		SESSIONID SessionID = GiveID();
		m_SessionContainer.emplace(MakePair(SessionID, _spSession));
		__super::InsertSession(_spSession);
	}

	SESSIONID UClientService::GiveID()
	{
		RETURN_CHECK(false == m_RemainIDStack.IsEmpty(), m_RemainIDStack.Pop());
		return GetCurSessionCount();
	}

	void UClientService::Free()
	{
		for (auto& iter : m_SessionContainer)
		{
			iter.second->Disconnect();
		}
	}
}