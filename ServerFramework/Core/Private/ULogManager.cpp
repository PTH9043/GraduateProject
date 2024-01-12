#include "CoreDefines.h"
#include "ULogManager.h"

namespace Core {

	ULogManager::ULogManager() : 
		m_LogFileStream{"..\\Bin\\Log.txt",  std::ofstream::out | std::ofstream::app }
	{
	}

	void ULogManager::PrintOut(const char* _string)
	{
		printf_s("%s", _string);
	}

	void ULogManager::FileOut(const char* _string)
	{
		m_LogFileStream << _string;
		m_LogFileStream.flush();
	}

	void ULogManager::Free()
	{
		m_LogFileStream.close();
	}
}
