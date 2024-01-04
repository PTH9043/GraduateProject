#include "CoreDefines.h"
#include "UServerService.h"
#include "USession.h"
#include "UCoreInstance.h"

namespace Core {

	UServerService::UServerService(OBJCON_CONSTRUCTOR) : 
		UService(OBJCON_CONDATA, SERVICETYPE::SERVER), 
		m_TcpAcceptor{ GetIOContext(),Asio::ip::tcp::endpoint(Asio::ip::make_address(IP_ADDRESS),
	TCP_PORT_NUM) }
	{
	}

	_bool UServerService::NativeConstruct()
	{
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
		RETURN_CHECK(0 >= m_SessionContainer.size(), ;);
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
	}

	void UServerService::InsertSession(SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		SESSIONID SessionID = _spSession->GetID();
		m_SessionContainer.emplace(MakePair(SessionID, _spSession));
	}

	void UServerService::ThreadFunc(void* _spService)
	{
		RETURN_CHECK(nullptr == _spService, ;);

		UServerService* pService = static_cast<UServerService*>(_spService);
		IOContext& context = pService->GetIOContext(REF_RETURN);
		// Running 
		while (true)
		{
			// ½ÇÇà 
			context.run();
		}
	}

	void UServerService::Free()
	{
		LOCKGUARD<MUTEX> M{ GetLastLock()};

		for (auto& iter : m_SessionContainer)
		{
			iter.second->Disconnect();
		}
		m_TcpAcceptor.close();
	}
}