#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASQLLOGINTABLE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASQLLOGINTABLE_H

#include "AMySqlTable.h"

BEGIN(Core)

class ASqlLoginTable final : public AMySqlTable {
public:
	ASqlLoginTable();
	DESTRUCTOR(ASqlLoginTable)
public:
	// ExcuteQueryMessage
	virtual _bool ExcuteQueryMessage(SHPTR<AMySqlConnector> _spMySqlConnector, SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData, ...) override;
protected:
	// Create Table
	virtual _bool CreateTable(const std::unique_ptr<sql::Statement>& _stmt) override;
private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASQLLOGINTABLE_H