#include "CoreDefines.h"
#include "CoreSqlDefines.h"
#include "AMySqlDriver.h"
#include "AMySqlConnector.h"

namespace Core {
	AMySqlDriver::AMySqlDriver() : 
		m_pDriver{ nullptr }, m_MySqlArray{}
	{
	}

	_bool AMySqlDriver::NativeConstruct(const _string& _strAddress, const _string& _strName, 
		const _string& _strPassward)
	{
		// Connect My Sql 
		m_pDriver = sql::mysql::get_driver_instance();
		RETURN_CHECK(nullptr == m_pDriver, false);
		// MySql Ready
		for (auto& iter : m_MySqlArray)
		{
			iter = CreateInitNative<AMySqlConnector>(m_pDriver, _strAddress, _strName, _strPassward);
			RETURN_CHECK(nullptr == iter, false);
		}
		return true;
	}

	void AMySqlDriver::Free() {
		delete m_pDriver;
	}
}