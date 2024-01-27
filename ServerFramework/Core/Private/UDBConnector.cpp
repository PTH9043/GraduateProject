#include "CoreDefines.h"
#include "UDBConnector.h"

namespace Core {
	UDBConnector::UDBConnector() : 
		m_pDriver{nullptr}, m_pConn{nullptr}
	{
	}

	_bool UDBConnector::NativeConstruct(const _string& _strAddress, const _string& _strName, 
		const _string& _strPassward)
	{
		// Connect My Sql 
		m_pDriver = sql::mysql::get_driver_instance();
		RETURN_CHECK(nullptr == m_pDriver, false);
		m_pConn = m_pDriver->connect(_strAddress.c_str(), _strName.c_str(), _strPassward.c_str());
		RETURN_CHECK(nullptr == m_pConn, false);

		return true;
	}

	void UDBConnector::Free()
	{
		delete m_pConn;
		delete m_pDriver;
	}
}