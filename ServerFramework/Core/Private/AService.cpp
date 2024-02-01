#include "CoreDefines.h"
#include "AService.h"
#include "ASession.h"

namespace Core {

	AService::AService(OBJCON_CONSTRUCTOR, SERVICETYPE _Type) :
		ACoreObject(OBJCON_CONDATA), m_ServiceType{_Type}, 
		m_IOContext{TLS::MAX_THREAD},
		m_TcpSocket{ m_IOContext },
		m_CurrentSessionCount{0},
		m_IDIssuance{0}
	{
	}

	_bool AService::Start()
	{
		return true;
	}


	void AService::LeaveService(const SESSIONID _SessionID)
	{
	}

	void AService::InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession)
	{
	}

	SESSIONID AService::GiveID()
	{
		return m_IDIssuance.fetch_add(1);
	}

	void AService::Free()
	{
		m_TcpSocket.close();
		m_IOContext.stop();
	}
}
