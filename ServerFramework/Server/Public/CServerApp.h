#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP_H

#include "UBase.h"

BEGIN(Core)
class UCoreInstance;
END

BEGIN(Server)
class CServerAdiminstor;
/*
@ Date: 2023-12-26, Writer: 박태현
@ Explain
- ServerApp을 관리하는 클래스
*/
class CServerApp : public Core::UBase {
public:
	CServerApp();
	NO_COPY(CServerApp)
	DESTRUCTOR(CServerApp)
public:
	void Init();
private:
	virtual void Free() override;

private:
	Core::SHPTR<Core::UCoreInstance>	m_spCoreInstance;
	Core::SHPTR< CServerAdiminstor>		m_spServerAdiminster;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP_H