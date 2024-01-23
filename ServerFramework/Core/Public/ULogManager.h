#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ULOGMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ULOGMANAGER_H

#include "UBase.h"

BEGIN(Core)

class  ULogManager final : public UBase  {
public:
	ULogManager();
	NO_COPY(ULogManager)
	DESTRUCTOR(ULogManager)
public:
	void PrintOut(const char* _string);
	void FileOut(const char* _string);
private:
	virtual void Free() override;

private:
	std::ofstream		m_LogFileStream;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ULOGMANAGER_H