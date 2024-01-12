#include "CoreDefines.h"
#include "UApp.h"

namespace Core
{
	UApp::UApp() :
		m_spCoreInstance{ Create<UCoreInstance>() },
		 m_spService{nullptr}
	{
#ifdef _WIN32
		_wsetlocale(LC_ALL, L"korean");
#else
		setlocale(LC_ALL, "ko_KR.utf8");
#endif
	}

	UApp::~UApp()
	{
	}

	_bool UApp::Start()
	{

		return true;
	}
}