#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H

#include "UObject.h"

BEGIN(Core)

class CACHE_ALGIN_CORE_DLL  UAwsConnector abstract : public UObject {
public:
	UAwsConnector(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(UAwsConnector)
public:
	virtual _bool Start() PURE;

private:
	virtual void Free() override;

private:
	// Options
	Aws::SDKOptions m_Options;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONNECTOR_H
