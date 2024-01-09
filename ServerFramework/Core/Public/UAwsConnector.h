#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H

#include "UObject.h"

BEGIN(Core)

class CACHE_ALGIN_CORE_DLL UAwsConnector abstract : public UObject {
public:
	UAwsConnector(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(UAwsConnector)
public:
	virtual _bool NativeConstruct() PURE;


	_bool IsAwsActivated() const { return m_isAwsActivated; }
protected:
	virtual void OnStartGameSession(Aws::GameLift::Server::Model::GameSession _Session) PURE;
	virtual void ProcessTerminate() PURE;

private:
	virtual void Free() override;
private:
	_bool		m_isAwsActivated;
	_uint		m_CheckTerminationCount;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H