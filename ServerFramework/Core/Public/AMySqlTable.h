#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLTABLE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLTABLE_H

#include "ACoreBase.h"

BEGIN(Core)
class AMySqlConnector;
/*
@ Date: 2024-01-28, Writer: 박태현
@ Explain
-  Table 별로 데이터를 관리하기 위한 클래스이다. 
*/
class AMySqlTable abstract : public ACoreBase {
public:
	AMySqlTable(SQLTABLETYPE _sqlTableType);
	DESTRUCTOR(AMySqlTable)
public:
	_bool NativeConstruct(SHPTR<AMySqlConnector> _spMySqlConnector, const _string& _strTableName);
	// ExcuteQueryMessage
	virtual _bool ExcuteQueryMessage(SQLQUERYTYPE _QueryType, const _string& _strQueryData, ...) PURE;
public: /* get set*/
	const SQLTABLETYPE GetTableType() const { return m_eSqlTableType; }
	const _string& GetTableName() const { return m_strTableName; }
protected:
	// Create Table
	virtual _bool CreateTable(const std::unique_ptr<sql::Statement>& _Unique) PURE;
protected: /* get set*/
	sql::Statement* GetStatement() const { return m_upStatement.get(); }
	sql::PreparedStatement* GetPrepareStatement() const { return m_upPrepareStatement.get(); }
private:
	virtual void Free() override;
private: // prepare
	SQLTABLETYPE													m_eSqlTableType;
	_string																	m_strTableName;
	std::unique_ptr<sql::Statement> 					m_upStatement;
	std::unique_ptr<sql::PreparedStatement>	m_upPrepareStatement;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLTABLE_H