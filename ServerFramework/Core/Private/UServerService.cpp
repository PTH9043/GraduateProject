#include "CoreDefines.h"
#include "UServerService.h"
#include "USession.h"
#include "UCoreInstance.h"
#include "UNavigation.h"

namespace Core {

	UServerService::UServerService(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath) :
		UService(OBJCON_CONDATA, SERVICETYPE::SERVER), 
		m_TcpAcceptor{ GetIOContext(),Asio::ip::tcp::endpoint(Asio::ip::tcp::v4(),TCP_PORT_NUM) }, 
		m_spNavigation{nullptr}
	{

	}

	_bool UServerService::NativeConstruct()
	{
		return true;
	}

	_bool UServerService::Start()
	{
		__super::Start();
		SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();

		for (_uint i = 0; i < TLS::MAX_THREAD; ++i)
		{
			spCoreInstance->RegisterFunc(UServerService::ThreadFunc, GetIOConectPointer());
		}
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
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			if (iter->second)
			{
				iter->second->Disconnect();
				iter->second.reset();
			}
	//		m_SessionContainer.unsafe_erase(iter);
		}
	}

	void UServerService::InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		m_SessionContainer.insert(MakePair(_SessionID, _spSession));
	}

	void UServerService::ThreadFunc(void* _spService)
	{
		RETURN_CHECK(nullptr == _spService, ;);

		IOContext* pService = static_cast<IOContext*>(_spService);
		// Running 
		// ½ÇÇà 
		pService->run();
	}

	void UServerService::Free()
	{
		for (auto& iter : m_SessionContainer)
		{
			iter.second->Disconnect();
		}
		m_TcpAcceptor.close();
	}
}