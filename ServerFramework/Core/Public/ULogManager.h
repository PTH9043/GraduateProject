#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ULOGMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ULOGMANAGER_H

#include "UBase.h"
#include <fstream>

BEGIN(Core)

class CACHE_ALGIN ULogManager final : public UBase  {
public:
	ULogManager();
	NO_COPY(ULogManager)
	DESTRUCTOR(ULogManager)
public:
	

private:
	virtual void Free() override;

private:
	std::ofstream		m_LogFileStream;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ULOGMANAGER_H