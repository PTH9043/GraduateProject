#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USQLDBQUERY_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USQLDBQUERY_H

#include "UBase.h"

BEGIN(Core)

class CORE_DLL USqlDBQuery final : public UBase {
public:
	USqlDBQuery(const _string& _strQueryName);

private:

};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USQLDBQUERY_H