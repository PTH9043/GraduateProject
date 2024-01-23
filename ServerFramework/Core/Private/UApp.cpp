#include "CoreDefines.h"
#include "UApp.h"
#include "UCoreInstance.h"

namespace Core
{
	UApp::UApp() :
		m_spCoreInstance{ Create<UCoreInstance>() }
	{
#ifdef _WIN32
		_wsetlocale(LC_ALL, L"korean");
#else
		setlocale(LC_ALL, "ko_KR.utf8");
#endif
	}

	UApp::~UApp(){
		m_spCoreInstance.reset();
	}

	_bool UApp::Start()
	{

		return true;
	}
}