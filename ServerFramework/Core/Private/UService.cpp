#include "CoreDefines.h"
#include "UService.h"
#include "USession.h"

namespace Core {

	UService::UService(OBJCON_CONSTRUCTOR, SERVICETYPE _Type) :
		UObject(OBJCON_CONDATA), m_ServiceType{_Type}, 
		m_TcpSocket{ m_IOContext },
		m_CurrentSessionCount{0},
		m_IDIssuance{0}, 
		m_isAwsActivation{false}
	{
	}

	_bool UService::Start()
	{
		return true;
	}


	void UService::LeaveService(const SESSIONID _SessionID)
	{
		auto iter = Aws::GameLift::Server::InitSDK();
	}

	void UService::InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession)
	{
	}

	SESSIONID UService::GiveID()
	{
		return m_IDIssuance.fetch_add(1);
	}

	void UService::Free()
	{
		m_TcpSocket.close();
		m_IOContext.stop();
	}
}
