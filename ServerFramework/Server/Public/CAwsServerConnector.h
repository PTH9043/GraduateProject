#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CAWSSERVERCONNECTOR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CAWSSERVERCONNECTOR_H

#include "UAwsConnector.h"

BEGIN(Server)

class CACHE_ALGIN CAwsServerConnector final : public UAwsConnector {
public:
	CAwsServerConnector(OBJCON_CONSTRUCTOR);
	NO_COPY(CAwsServerConnector)
	DESTRUCTOR(CAwsServerConnector)
public:
	virtual _bool Start() override;
protected:
	virtual void StartGameSession() override;
	virtual void TerminateGameSession(_int _exitCode) override;
private:
	virtual void Free() override;
private:
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CAWSSERVERCONNECTOR_H
