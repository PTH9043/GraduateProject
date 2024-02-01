#include "CoreDefines.h"
#include "ALogManager.h"

namespace Core {

	ALogManager::ALogManager() : 
		m_LogFileStream{"..\\Bin\\Log.txt",  std::ofstream::out | std::ofstream::app }
	{
	}

	void ALogManager::PrintOut(const char* _string)
	{
		printf_s("%s", _string);
	}

	void ALogManager::FileOut(const char* _string)
	{
		m_LogFileStream << _string;
		m_LogFileStream.flush();
	}

	void ALogManager::Free()
	{
		m_LogFileStream.close();
	}
}
