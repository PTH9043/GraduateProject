#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H

#include "UBase.h"

BEGIN(Core)

class CACHE_ALGIN_CORE_DLL UPathFinder final : public UBase {
public:
	UPathFinder();
	NO_COPY(UPathFinder)
	DESTRUCTOR(UPathFinder)
public:



private:
	virtual void Free() override;
private:

};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UPATHFINDER_H