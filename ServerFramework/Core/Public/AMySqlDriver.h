#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H

#include "ACoreBase.h"

BEGIN(Core)
class AMySqlConnector;
/*
@ Date: 2023-01-28, Writer: 박태현
@ Explain
-  DataBase와 연결을 하기 위한 클래스이다. 
연결할 데이터베이스는 mysql, redis
*/
class CORE_DLL AMySqlDriver final : public ACoreBase {
public:
	using SQLARRAY = ARRAY<SHPTR<AMySqlConnector>, TLS::MAX_WORKTHREAD>;
public:
	AMySqlDriver();
	NO_COPY(AMySqlDriver)
	DESTRUCTOR(AMySqlDriver)
public:
	_bool	NativeConstruct(const _string& _strAddress, const _string& _strName, const _string& _strPassward);
private:
	virtual void Free() override;
private:
	sql::mysql::MySQL_Driver*	 m_pDriver;
	SQLARRAY									 m_MySqlArray;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H