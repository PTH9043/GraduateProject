#include "CoreDefines.h"
#include "AMySqlTable.h"
#include "AMySqlConnector.h"

namespace Core
{
	AMySqlTable::AMySqlTable(SQLTABLETYPE _eTableType) :m_eSqlTableType{ _eTableType },
		m_strTableName { "" }, m_upStatement{ nullptr }, m_upPrepareStatement{nullptr}
	{
	}

	_bool AMySqlTable::NativeConstruct(SHPTR<AMySqlConnector> _spMySqlConnector, const _string& _strTableName)
	{
		assert(nullptr != _spMySqlConnector);

		std::unique_ptr<sql::Statement> State(_spMySqlConnector->MakeStatement());
		std::unique_ptr<sql::PreparedStatement> PrepareState(_spMySqlConnector->MakePrepareStatement());

		_string tableName = _strTableName;
		_string dbName = SQL_DATABASE_NAME;
		_string checkTableExistsQuery = "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = '"
			+ dbName + "' AND table_name = '" + tableName + "'";

		std::unique_ptr<sql::ResultSet> Res(State->executeQuery(checkTableExistsQuery.c_str()));
		assert(Res != nullptr);
		// 만약 데이터베이스에 테이블이 존재하지 않는다면
		if (false == Res->next() || Res->getInt(1) < 0)
		{
			// Table을 생성한다. 
			return CreateTable(State);
		}
		return false;
	}

	void AMySqlTable::Free()
	{
	}

}