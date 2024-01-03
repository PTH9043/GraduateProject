#include "CoreDefines.h"
#include "UService.h"
#include "USession.h"

namespace Core {

	IMPLEMENT_ONCEDATA(UService)

	UService::UService(OBJCON_CONSTRUCTOR, SERVICETYPE _Type) :
		UObject(OBJCON_CONDATA), m_ServiceType{_Type}, 
		m_TcpAcceptor{ m_IOContext,Asio::ip::tcp::endpoint(Asio::ip::make_address(IP_ADDRESS), 
			TCP_PORT_NUM) },
		m_TcpSocket{ m_IOContext },
		m_UdpSocket{ m_IOContext, Asio::ip::udp::endpoint(Asio::ip::make_address(IP_ADDRESS),
			UDP_PORT_NUM) },
		m_CurrentSessionCount{0}
	{
		CREATEINSTANCE_ONCEDATA;
	}


	void UService::LeaveService(const SESSIONID _SessionID)
	{
		--m_CurrentSessionCount;
	}

	void UService::InsertSession(SHPTR<USession> _spSession)
	{
		++m_CurrentSessionCount;
	}

	SESSIONID UService::GiveID()
	{
		while (true)
		{
			SESSIONID id = m_IDIssuance + 1;
			if (true == Core::CAS(&m_IDIssuance, m_IDIssuance, id))
			{
				break;
			}
		}
		return m_IDIssuance;
	}

	void UService::Free()
	{
		LOCKGUARD<MUTEX> M{ m_LastLock };

		m_TcpSocket.close();
		m_UdpSocket.close();
		m_TcpAcceptor.close();

		m_IOContext.stop();
	}
}
