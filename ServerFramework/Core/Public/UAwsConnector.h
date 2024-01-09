#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H

#include "UObject.h"


BEGIN(Core)

class CACHE_ALGIN_CORE_DLL UAwsConnector abstract : public UObject {
public:
	using AWSGAMESESSION = Aws::GameLift::Server::Model::GameSession;
public:
	UAwsConnector(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(UAwsConnector)
public:
	virtual _bool NativeConstruct() PURE;
public: /* Get Set */
	_bool IsAwsActivated() const { return m_isAwsActivated; }

protected:
	void OnStartGameSession(Aws::GameLift::Server::Model::GameSession _Session);
	void OnProcessTerminate();

	virtual void StartGameSession() PURE;
	virtual void TerminateGameSession(_int _exitCode) PURE;
private:
	virtual void Free() override;
private:
	_bool								m_isAwsActivated;
	_uint								m_CheckTerminationCount;
	AWSGAMESESSION		m_AwsGameSession;
	Aws::SDKOptions			m_SDKOptions;

	UFastSpinLock				m_FastSpinLock;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H