#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UDBCONNECTOR
#define _SERVERFRAMEWORK_CORE_PUBLIC_UDBCONNECTOR

#include "UBase.h"

BEGIN(Core)

/*
@ Date: 2023-01-26, Writer: 박태현
@ Explain
-  DataBase와 연결을 하기 위한 클래스이다. 
연결할 데이터베이스는 mysql, redis
*/
class CORE_DLL UDBConnector final : public UBase {
public:
	UDBConnector();
	NO_COPY(UDBConnector)
	DESTRUCTOR(UDBConnector)
public:
	_bool	NativeConstruct(const _string& _strAddress, const _string& _strName, const _string& _strPassward);
private:
	sql::mysql::MySQL_Driver*	m_pDriver;
	sql::Connection*						m_pConn;
private:
	virtual void Free() override;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UDBCONNECTOR