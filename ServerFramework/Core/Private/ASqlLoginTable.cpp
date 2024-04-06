#include "CoreDefines.h"
#include "ASqlLoginTable.h"
#include "AMySqlConnector.h"


BEGIN(Core)

ASqlLoginTable::ASqlLoginTable() : AMySqlTable(SQLTABLETYPE::LOGIN)
{
}

_bool ASqlLoginTable::ExcuteQueryMessage(SHPTR<AMySqlConnector> _spMySqlConnector, 
	SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData, ...)
{

	return _bool();
}

_bool ASqlLoginTable::CreateTable(const std::unique_ptr<sql::Statement>& _stmt)
{
	assert(nullptr != _stmt);

	_string CheckExistDatabaseQuery = CombineRawString("SHOW TABLES LIKE'", SQL_LOGINTABLE_NAME) + "'";
	std::unique_ptr<sql::ResultSet> res(_stmt->executeQuery(CheckExistDatabaseQuery.c_str()));
	assert(res != nullptr);
	if (false == res->next())
	{
		_string CreateTableQuery = "CREATE TABLE users ("
												         "id INT AUTO_INCREMENT PRIMARY KEY, "
												         "username VARCHAR(255) NOT NULL, "
												 		"password VARCHAR(255) NOT NULL)";

		std::unique_ptr<sql::ResultSet> MakeDB(_stmt->executeQuery(CreateTableQuery.c_str()));
		assert(MakeDB != nullptr);
		return true; 
	}
	return false;
}

void ASqlLoginTable::Free()
{
}


END