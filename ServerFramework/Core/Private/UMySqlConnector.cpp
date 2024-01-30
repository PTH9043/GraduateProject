#include "CoreDefines.h"
#include "UMySqlConnector.h"

namespace Core {

	UMySqlConnector::UMySqlConnector() : m_pConnection{nullptr}{
	}

	_bool UMySqlConnector::NativeConstruct(sql::mysql::MySQL_Driver* _pDriver, const _string& _strAddress, 
		const _string& _strName, const _string& _strPassward)
	{
		RETURN_CHECK(nullptr == _pDriver, false);
		m_pConnection = _pDriver->connect(_strAddress.c_str(), _strName.c_str(), _strPassward.c_str());
		return nullptr != m_pConnection;
	}

	sql::Statement* UMySqlConnector::MakeStatement(){
		return m_pConnection->createStatement();
	}

	sql::PreparedStatement* UMySqlConnector::MakePrepareStatement(const _string& _strStateString){
		return m_pConnection->prepareStatement(_strStateString.c_str());
	}

	void UMySqlConnector::Free()
	{
		delete m_pConnection;
	}
}