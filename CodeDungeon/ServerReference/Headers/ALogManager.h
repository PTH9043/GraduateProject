#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ALOGMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ALOGMANAGER_H

#include "ACoreBase.h"

BEGIN(Core)

class  ALogManager final : public ACoreBase  {
public:
	ALogManager();
	NO_COPY(ALogManager)
	DESTRUCTOR(ALogManager)
public:
	void PrintOut(const char* _string);
	void FileOut(const char* _string);
private:
	virtual void Free() override;

private:
	std::ofstream		m_LogFileStream;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ALOGMANAGER_H