#include "CoreDefines.h"
#include "AMySqlConnector.h"

namespace Core {

	AMySqlConnector::AMySqlConnector() : m_pConnection{ nullptr }, m_iThreadIndex{ 0 } {
	}

	_bool AMySqlConnector::NativeConstruct(sql::mysql::MySQL_Driver* _pDriver, const _string& _strAddress, 
		const _string& _strName, const _string& _strPassward, _int _iThreadID)
	{
		assert(nullptr != _pDriver);
		m_pConnection = _pDriver->connect(_strAddress.c_str(), _strName.c_str(), _strPassward.c_str());
		m_iThreadIndex = _iThreadID;

		if (nullptr == m_pConnection)
			return false;

		m_pConnection->setSchema("CodeDungeon");
		return true;
	}

	sql::Statement* AMySqlConnector::MakeStatement(){
		return m_pConnection->createStatement();
	}

	sql::PreparedStatement* AMySqlConnector::MakePrepareStatement(const _string& _strStateString){
		return m_pConnection->prepareStatement(_strStateString.c_str());
	}

	void AMySqlConnector::Free()
	{
		delete m_pConnection;
	}
}